/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.h
 - Class / enums / API
******************************************************************/

#include "VEDirect.h"

VEDirect::VEDirect(HardwareSerial& port):
	VESerial(port)
	// Initialise the serial port that the
	// VE.Direct device is connected to and
	// store it for later use.
{
}

VEDirect::~VEDirect() {
	// virtual destructor
}

uint8_t VEDirect::begin() {
	// Check connection the serial port
	VESerial.begin(19200);
	if (VESerial) {
		delay(500);
		if(VESerial.available()) {
			VESerial.flush();
			VESerial.end();
			return 1;
		}
	}
	return 0;
}

int32_t VEDirect::read(uint8_t target) {
	// Read VE.Direct text blocks from the serial port
	// Search for the label specified by enum target_label
	// Extract and return the corresponding value.

	int32_t ret = 0;					// The value to be returned
	char VE_line[VED_LINE_SIZE];		// Line buffer
	char* label;
	char* value;
	uint8_t buf_idx = 0;

	const char delim[2] = "\t";			// Delim between label and value
	// Simple state machine as to navigate the
	// flow of text data that that is sent every second
	uint8_t block_count = 0;
	uint8_t cr = 0;
	uint8_t checksum = 0;

	uint8_t b;							// byte read from the stream

	VESerial.begin(19200);

	if (VESerial) {
		// BMV continuously transmits 2x text data blocks to deliver all data.
		// Read 3x times, discarding the first (likely) partial block,
		// to get the two complete data blocks.
		while (block_count < 3) {
			if (VESerial.available()) {
				// Get the next byte from the serial stream
				b = VESerial.read();
				switch (b) {
					case '\n':  // start of newline - reset read buffer
						cr = 0;
						VE_line[0] = '\0';
						buf_idx = 0;
						break;
					case '\r': // eol - terminate the buffer
						cr = 1;
						VE_line[buf_idx] = '\0';
						buf_idx++;
						break;
					default:
						// Is the checksum expected?
						if (checksum) {
							// TODO: Capture it and use it later
							// Currently: just ignore it and the preceding \t
							// Assume eol, reset and increment the block_count
							if (b != '\t') {
								// then it a checksum byte
								cr = 0;
								VE_line[0] = '\0';
								buf_idx = 0;
								block_count++;
								checksum = 0;
							} // no else - was the \t before, ignore
						} else {
							// Normal char of interest
							// Clear cr flag which may have been set
							cr = 0;
							// Add the char to the buffer
							VE_line[buf_idx] = b;
							buf_idx++;
							// Check for the Checksum label
							// Turn on flag to trigger checksum
							// \t and byte capture on next loops
							if (strncmp(VE_line, "Checksum", 8) == 0) {
								VE_line[8] = '\0';
								checksum = 1;
							}
						}
				}
			}
			// Evaluate the flags and buffer contents
			if (cr && buf_idx) {
				// whole line in buffer
				label = strtok(VE_line, delim);
				value = strtok(0, delim);
				// Look for the label passed requested on he call
				switch (target) {
					case VE_SOC:
						if (strcmp(label, "SOC") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_VOLTAGE:
						if (strcmp(label, "V") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_POWER:
						if (strcmp(label, "P") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_CURRENT:
						if (strcmp(label, "I") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_POWER_PV:
						if (strcmp(label, "PPV") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_VOLTAGE_PV:
						if (strcmp(label, "VPV") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_YIELD_TOTAL:
						if (strcmp(label, "H19") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_YIELD_TODAY:
						if (strcmp(label, "H20") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_YIELD_YESTERDAY:
						if (strcmp(label, "H22") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_POWER_MAX_TODAY:
						if (strcmp(label, "H21") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_ERROR:
						if (strcmp(label, "ERR") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					case VE_STATE:
						if (strcmp(label, "CS") == 0) {
							sscanf(value, "%ld", &ret);
							return ret;
						}
						break;
					default:
						break;
				}
			}
		}
		// Tidy up
		VESerial.flush();
		VESerial.end();
	}
	return ret;
}

void VEDirect::copy_raw_to_serial0() {
	// Read VE.Direct text blocks from the serial port
	// Buffer them and then print them to Serial
	// *******
	// NOTE: 	Do not use this function for anything serious
	//			To allow lower Serial0 speed (eg 9600) it buffers
	//			the input from the VE device (at 19200) and then prints it
	// 			It is memory / malloc ugly and without fail safes
	//			Only useful for	low level port dumping

	typedef struct BUFFER {
		char* line;
		struct BUFFER* next;
	} bufline;

	bufline* head = (bufline *)malloc(sizeof(bufline));
	head->next = NULL;
	bufline* walker = head;

	char VE_line[VED_LINE_SIZE];		// Line buffer
	uint8_t buf_idx = 0;

	// Simple state machine as to navigate the
	// flow of text data that that is sent every second
	uint8_t block_count = 0;
	uint8_t newline = 0;

	uint8_t b;							// byte read from the stream

	VESerial.begin(19200);

	if (VESerial) {
		// BMV continuously transmits 2x text data blocks to deliver all data.
		// Read 3x times, discarding the first (likely) partial block,
		// to get the two complete data blocks.
		while (block_count < 15) {
			if (VESerial.available()) {
				// Get the next byte from the serial stream
				b = VESerial.read();
				switch (b) {
					case '\r':
						break;
					case '\n': // terminate the buffer
						newline = 1;
						VE_line[buf_idx] = '\0';
						break;
					default:
						// Normal char of interest
						newline = 0;
						// Add the char to the buffer
						VE_line[buf_idx] = b;
						buf_idx++;

						if (strncmp(VE_line, "Checksum", 8) == 0) {
							VE_line[8] = '\0';
							block_count++;
						}

				}
			}
			// Evaluate the flags and buffer contents
			if (newline && buf_idx) {
				// whole line in buffer
				walker->line = (char *)malloc(strlen(VE_line) +10);
				strcpy(walker->line, VE_line);
				walker->next =(bufline *)malloc(sizeof(bufline));
				walker = walker->next;
				walker->next = NULL;

				newline = 0;
				VE_line[0] = '\0';
				buf_idx = 0;

			}
		}
		// print it all
		Serial.println("Out of collect loop");
		walker = head;
		while (walker->next != NULL) {
			Serial.println(walker->line);
			free(walker->line);
			walker = walker->next;
			free(head);
			head = walker;
		}
		free(head);

		// Tidy up
		VESerial.flush();
		VESerial.end();
	}
}

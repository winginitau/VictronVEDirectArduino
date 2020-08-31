/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.cpp
 - Implementation
 Updates:
 - 2019-07-14 See VEDirect.h
 - 2020-04-10 Converted to SoftwareSerial, added checks for null pointers for ESP8266
 - 2020-08-31 Removed errant constructor
******************************************************************/

#include "VEDirect.h"

VEDirect::VEDirect(byte rxPin, byte txPin):
	VESerial(*new SoftwareSerial(rxPin, txPin))
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
	VESerial.begin(VED_BAUD_RATE);
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
	// Read VE.Direct lines from the serial port
	// Search for the label specified by enum target
	// Extract and return the corresponding value
	// If value is "ON" return 1. If "OFF" return 0;

	uint16_t loops = VED_MAX_READ_LOOPS;
	uint8_t lines = VED_MAX_READ_LINES;
	int32_t ret = 0;					// The value to be returned
	char line[VED_LINE_SIZE] = "\0";	// Line buffer
	uint8_t idx = 0;					// Line buffer index
	char* label;
	char* value_str;
	int8_t b;							// byte read from the stream

	VESerial.begin(VED_BAUD_RATE);

	while (lines > 0) {
		if (VESerial.available()) {
			while (loops > 0) {
				b = VESerial.read();
				if ((b == -1) || (b == '\r')) { 	// Ignore '\r' and empty reads
					loops--;
				} else {
					if (b == '\n') { 				// EOL
						break;
					} else {
						if (idx < VED_LINE_SIZE) {
							line[idx++] = b;		// Add it to the buffer
						} else {
							return 0;				// Buffer overrun
						}
					}
				}
			}
			line[idx] = '\0';						// Terminate the string

			// Line in buffer
			if (target == VE_DUMP) {
				// Diagnostic routine - just print to Serial0;
				Serial.println(line);
				// Continue on rather than break to reset for next line
			}

			label = strtok(line, "\t");
			if (label) {
				if (strcmp_P(label, ved_labels[target]) == 0) {
					value_str = strtok(0, "\t");
					if (value_str) {
						if (value_str[0] == 'O') { 		//ON OFF type
							if (value_str[1] == 'N') {
								ret = 1;	// ON
								break;
							} else {
								ret = 0;	// OFF
								break;
							}
						} else {
							sscanf(value_str, "%ld", &ret);
							break;
						}
					}	
				} else {			// Line not of interest
					lines--;
					loops = VED_MAX_READ_LOOPS;
					line[0] = '\0';
					idx = 0;
				}
			}	
		}
	}
	return ret;
}

void VEDirect::copy_raw_to_serial0() {
	read(VE_DUMP);
}


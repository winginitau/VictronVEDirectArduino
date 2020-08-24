/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, 2020 Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.h
 - Class / enums / API

 Updates:
 - 2019-07-14:
  	  - Rewrite of read - cleaner.
  	  - Target labels extendible with enum and PROGMEM strings
  	  - Retired copy_raw_to_serial0() code - use VE_DUMP on read
  	  - Added some tunable parameters see #defines
 - 2020-08-24 - Contribution of Rickard Nordström Pettersson
  	  - Added VE_SOC, VE_POWER, VE_ALARM for BMW devices
******************************************************************/

#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include <Arduino.h>

// Tunable parameters - defaults tested on mega2560 R3
#define VED_LINE_SIZE 40		 // Seems to be plenty. VE.Direct protocol could change
#define VED_MAX_LEBEL_SIZE 20	 // Max length of all labels of interest + '\0'. See ved_labels[]
#define VED_MAX_READ_LOOPS 60000 // How many read loops to be considered a read time-out
#define VED_MAX_READ_LINES 50	 // How many lines to read looking for a value
								 // before giving up. Also determines lines for diag dump
#define VED_BAUD_RATE 19200

// Extend this and ved_labels[] for needed inclusions
enum VE_DIRECT_DATA {
	VE_DUMP = 0,
	VE_FW,
	VE_VOLTAGE,
	VE_CURRENT,
	VE_VOLTAGE_PV,
	VE_POWER_PV,
	VE_STATE,
	VE_MPPT,
	VE_ERROR,
	VE_LOAD,
	VE_YIELD_TOTAL,
	VE_YIELD_TODAY,
	VE_POWER_MAX_TODAY,
	VE_YIELD_YESTERDAY,
	VE_POWER_MAX_YESTERDAY,
	VE_DAY_SEQUENCE_NUMBER,
	VE_SOC,
	VE_POWER,
	VE_ALARM,
	VE_LAST_LABEL,
};

const char ved_labels[VE_LAST_LABEL][VED_MAX_LEBEL_SIZE] PROGMEM = {
		"Dump",	// a string that won't match any label
		"FW",
		"V",
		"I",
		"VPV",
		"PPV",
		"CS",
		"MPPT",
		"ERR",
		"LOAD",
		"H19",
		"H20",
		"H21",
		"H22",
		"H23",
		"HSDS",
		"SOC",
		"P",
		"Alarm"
};

class VEDirect {
public:
	VEDirect(HardwareSerial& port);
	virtual ~VEDirect();
	uint8_t begin();
	int32_t read(uint8_t target);
	void copy_raw_to_serial0(); // kept for backwards compatibility
private:
	HardwareSerial& VESerial;
};

#endif /* VEDIRECT_H_ */

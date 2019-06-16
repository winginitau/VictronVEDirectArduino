/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.h
 - Class / enums / API
******************************************************************/

#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include <Arduino.h>

#define VED_LINE_SIZE 20	// Seems to be plenty. VE.Direc protocol could change.

enum VE_DIRECT_DATA {
	VE_SOC = 0,
	VE_VOLTAGE,
	VE_POWER,
	VE_CURRENT
};

class VEDirect {
private:
	HardwareSerial& VESerial;
public:
	VEDirect(HardwareSerial& port);
	virtual ~VEDirect();
	uint8_t begin();
	int32_t read(uint8_t target);
	void copy_raw_to_serial0();
};




#endif /* VEDIRECT_H_ */

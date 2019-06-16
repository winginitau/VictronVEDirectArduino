/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: example.ino / example.cpp
 - Provides example use of the VEDirect library
******************************************************************/

#include "Arduino.h"
#include "VEDirect.h"

// 32 bit ints to collect the data from the device
int32_t VE_soc, VE_power, VE_voltage, VE_current;

// VEDirect instantiated with relevant serial object
VEDirect myve(Serial3);

void setup() {
	Serial.begin(9600);		// Adjust as needed
	Serial.println("Reading values from Victron Energy device using VE.Direct text mode");
}

void loop() {
	// Read the data
	if(myve.begin()) {									// test connection
		VE_soc = myve.read(VE_SOC);
		VE_power = myve.read(VE_POWER);
		VE_voltage = myve.read(VE_VOLTAGE);
		VE_current = myve.read(VE_CURRENT);
	} else {
		Serial.println("Could not open serial port to VE device");
		while (1);
	}

	// Print it each of the values
	Serial.println("Values Received");
	Serial.print("State of Charge (SOC): ");
	Serial.println(VE_soc, DEC);
	Serial.print("Power:                 ");
	Serial.println(VE_power, DEC);
	Serial.print("Voltage                ");
	Serial.println(VE_voltage, DEC);
	Serial.print("Current                ");
	Serial.println(VE_current, DEC);
	Serial.println();

	// Copy the raw data stream (excluding the checkdum line and byte) to Serial0
	Serial.println("All data from device (excluding checksum line)");
	myve.copy_raw_to_serial0();
	delay(10000);
	while(1);
}

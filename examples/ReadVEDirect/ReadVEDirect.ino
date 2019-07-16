/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: ReadVEDirect.ino / ReadVEDirect.cpp
 - Provides example use of the VEDirect library
******************************************************************/

#include "Arduino.h"
#include "VEDirect.h"

// 32 bit ints to collect the data from the device
int32_t VE_soc, VE_power, VE_voltage, VE_current;
// Boolean to collect an ON/OFF value
uint8_t VE_alarm;

// VEDirect instantiated with relevant serial object
VEDirect myve(Serial3);

void setup() {
	Serial.begin(9600);		// Adjust as needed
}

void loop() {
	Serial.println("Reading values from Victron Energy device using VE.Direct text mode");
	Serial.println();

	// Read the data
	if(myve.begin()) {									// test connection
		VE_soc = myve.read(VE_SOC);
		VE_power = myve.read(VE_POWER);
		VE_voltage = myve.read(VE_VOLTAGE);
		VE_current = myve.read(VE_CURRENT);
		VE_alarm = myve.read(VE_ALARM);
	} else {
		Serial.println("Could not open serial port to VE device");
		while (1);
	}

	// Print each of the values
	Serial.print("State of Charge (SOC): ");
	Serial.println(VE_soc, DEC);
	Serial.print("Power:                 ");
	Serial.println(VE_power, DEC);
	Serial.print("Voltage                ");
	Serial.println(VE_voltage, DEC);
	Serial.print("Current                ");
	Serial.println(VE_current, DEC);
	Serial.print("Alarm                  ");
	Serial.println(VE_alarm, DEC);
	Serial.println();

	// Copy the raw data stream (minus the \r) to Serial0
	// Call read() with a token that won't match any VE.Direct labels
	Serial.println("All data from device:");
	myve.read(VE_DUMP);
	Serial.println();
	delay(10000);
}

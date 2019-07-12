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
int32_t VE_soc, VE_power, VE_voltage, VE_current, VE_power_pv, VE_voltage_pv, VE_yield_total, VE_yield_today, VE_yield_yesterday, VE_power_max_today, VE_power_max_yesterday, VE_error, VE_state;

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
		VE_power_pv = myve.read(VE_POWER_PV);
		VE_voltage_pv = myve.read(VE_VOLTAGE_PV);
		VE_yield_total = myve.read(VE_YIELD_TOTAL);
		VE_yield_today = myve.read(VE_YIELD_TODAY);
		VE_yield_yesterday = myve.read(VE_YIELD_YESTERDAY);
		VE_power_max_today = myve.read(VE_POWER_MAX_TODAY);
		VE_power_max_yesterday = myve.read(VE_POWER_MAX_YESTERDAY);
		VE_error = myve.read(VE_ERROR);
		VE_state = myve.read(VE_STATE);
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
	Serial.print("Power PV               ");
	Serial.println(VE_power_pv, DEC);
	Serial.print("Voltage PV             ");
	Serial.println(VE_voltage_pv, DEC);
	Serial.print("Yield Total kWh        ");
	Serial.println(VE_yield_total, DEC);
	Serial.print("Yield Today kWh        ");
	Serial.println(VE_yield_today, DEC);
	Serial.print("Yield Yesterday kWh    ");
	Serial.println(VE_yield_yesterday, DEC);
	Serial.print("Max Power Today        ");
	Serial.println(VE_power_max_today, DEC);
	Serial.print("Max Power Yesterday    ");
	Serial.println(VE_power_max_yesterday, DEC);
	Serial.print("Error Code             ");
	Serial.println(VE_error, DEC);
	Serial.print("State of operation     ");
	Serial.println(VE_state, DEC);
	Serial.println();

	// Copy the raw data stream (excluding the checkdum line and byte) to Serial0
	Serial.println("All data from device (excluding checksum line)");
	myve.copy_raw_to_serial0();
	delay(10000);
	while(1);
}

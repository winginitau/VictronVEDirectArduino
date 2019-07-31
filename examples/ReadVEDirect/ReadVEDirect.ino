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
int32_t VE_fw, VE_voltage, VE_current, VE_voltage_pv, VE_power_pv, VE_state, VE_mppt,
		VE_error, VE_yield_total, VE_yield_today, VE_power_max_today, VE_yield_yesterday, 
		VE_power_max_yesterday, VE_day_sequence_number;
// Boolean to collect an ON/OFF value
uint8_t VE_load;

String CS0 = "Off";
String CS2 = "Fault";
String CS3 = "Bulk";
String CS4 = "Absorption";
String CS5 = "Float";
String ERR0 = "No error";
String ERR2 = "Battery voltage too high";
String ERR17 = "Charger voltage too high";
String ERR18 = "Charger over current";
String ERR20 = "Bulk time limit exceeded";
String ERR21 = "Current sensor issue";
String ERR26 = "Terminals overheated";
String ERR33 = "Input Voltage too high (solar panel)";
String ERR34 = "Input current too high (solar panel)";
String ERR38 = "Input shutdown (due to excessive battery voltage)";
String ERR116 = "Factory calibration lost";
String ERR117 = "invalied/incompatible firmware";
String ERR119 = "User settings invalid";
// VEDirect instantiated with relevant serial object
VEDirect myve(Serial2);

void setup() {
	Serial.begin(9600);		// Adjust as needed
}

void loop() {
	Serial.println("Reading values from Victron Energy device using VE.Direct text mode");
	Serial.println();

	// Read the data
	if(myve.begin()) {		// test connection
	    VE_fw = myve.read(VE_FW);
		VE_voltage = myve.read(VE_VOLTAGE);
		VE_current = myve.read(VE_CURRENT);
		VE_voltage_pv = myve.read(VE_VOLTAGE_PV);
		VE_power_pv = myve.read(VE_POWER_PV);
		VE_state = myve.read(VE_STATE);
		VE_mppt = myve.read(VE_MPPT);
		VE_error = myve.read(VE_ERROR);
		VE_load = myve.read(VE_LOAD);
		VE_yield_total = myve.read(VE_YIELD_TOTAL);
		VE_yield_today = myve.read(VE_YIELD_TODAY);
		VE_power_max_today = myve.read(VE_POWER_MAX_TODAY);
		VE_yield_yesterday = myve.read(VE_YIELD_YESTERDAY);
		VE_power_max_yesterday = myve.read(VE_POWER_MAX_YESTERDAY);
		VE_day_sequence_number = myve.read(VE_DAY_SEQUENCE_NUMBER);
		
	} else {
		Serial.println("Could not open serial port to VE device");
		//while (1);
	}

	// Print each of the values
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
	Serial.print("MPPT Code             ");
	Serial.println(VE_mppt, DEC);
	Serial.print("MPPT Firmware         ");
	Serial.println(VE_fw, DEC);
	Serial.print("Day Sequence Number   ");
	Serial.println(VE_day_sequence_number, DEC);
	Serial.print("Error Code             ");
	Serial.println(VE_error, DEC);
	Serial.print("Error Code             ");
	if (VE_error == 0){Serial.println(ERR0);}
	if (VE_error == 2){Serial.println(ERR2);}
	if (VE_error == 17){Serial.println(ERR17);}
	if (VE_error == 18){Serial.println(ERR18);}
	if (VE_error == 20){Serial.println(ERR20);}
	Serial.print("State of operation     ");
	Serial.println(VE_state, DEC);
	Serial.print("State of operation     ");
	if (VE_state == 0){Serial.println(CS0);}
	if (VE_state == 2){Serial.println(CS2);}
	if (VE_state == 3){Serial.println(CS3);}
	if (VE_state == 4){Serial.println(CS4);}
	if (VE_state == 5){Serial.println(CS5);}
	Serial.println();

	// Copy the raw data stream (minus the \r) to Serial0
	// Call read() with a token that won't match any VE.Direct labels
	Serial.println("All data from device:");
	myve.read(VE_DUMP);
	Serial.println();
	delay(10000);
}

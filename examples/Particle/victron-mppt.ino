/******************************************************************
 VE.Direct Arduino Example for Particle devices

 Copyright 2020, Rickard Nordström Pettersson
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: victron-mppt.ino
 - Provides example use of the VEDirect library on a Particle Photon
******************************************************************/

#include "VEDirect.h"

#define PUBLISH_EVENT_NAME "Victron-VEDirect"

char tmp[200];

// 32 bit ints to collect the data from the device
int32_t VE_fw, VE_voltage, VE_current, VE_voltage_pv, VE_power_pv, VE_state, VE_mppt,
		VE_error, VE_yield_total, VE_yield_today, VE_power_max_today, VE_yield_yesterday, 
		VE_power_max_yesterday, VE_day_sequence_number;

uint8_t VE_load;

VEDirect myve(Serial1);

void setup() {
	Serial.begin(9600);		// Adjust as needed
}

void loop() {
	// Read the data
	if(myve.begin()) {
		VE_voltage = myve.read(VE_VOLTAGE);
		VE_current = myve.read(VE_CURRENT);
		VE_voltage_pv = myve.read(VE_VOLTAGE_PV);
		VE_power_pv = myve.read(VE_POWER_PV);
		VE_state = myve.read(VE_STATE);
		VE_error = myve.read(VE_ERROR);

		sprintf(tmp ,"{ \"Voltage\": \"%d\", \"Current\": \"%d\", \"PowerPV\": \"%d\", \"VoltagePV\": \"%d\", \"ErrorCode\": \"%d\", \"StateOfOperation\": \"%d\" }\r\n", VE_voltage, VE_current, VE_power_pv, VE_voltage_pv, VE_error, VE_state);

        Particle.publish(PUBLISH_EVENT_NAME, tmp, 60, PRIVATE);
	} else {
		// Serial.println("Could not open serial port to VE device");
	}

	delay(60000);
}

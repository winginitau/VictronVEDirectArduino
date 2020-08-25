# Victron VE.Direct Arduino library
Light-weight Arduino library to read basic data using the VE.Direct protocol from Victron Energy

Built as part of a larger project, now making it available separately in case others find it useful.

Setup:
 - An Arduino(ish) board, works on Particle devices like Particle Photon and Particle Argon
 - A Victron Energy device that sends serial data using the text version of the VE.Direct protocol (little different values for different devices)
 - A 5v to 3.3v serial converter (BMV is 3.3v - don't plug it directly into an Arduino!)
 - Good to know is hat BMW-712 takes 3.3v in the VE.Direct port but the SmartSolar MPPT 75/15 need 5v to communicate on the VE.Direct port
 - Plugged into the Arduino on a serial port (eg Serial1, Serial2 etc)
 - See also: https://www.victronenergy.com/live/vedirect_protocol:faq
 - Distributed under an MIT license - see LICENCE.txt
 - Developed and tested with: MPPT 75/15, MPPT 100/30, BMW-712
 
Provides:
 - Access to the full protocol of VE.Direct variables like Power, Voltages, Panel Voltage and Panel Power depending on device.
 - A diagnostic "full dump" of everything coming from the device  

Code examples exists in the examples directory and the library you find in the src directory.

### Example of basic usage:
    #include "VEDirect.h"
    
    VEDirect my_bmv(Serial1);
    
    if my_bmv.begin() {
        int32_t my_int32 = my_bmv.read(VE_SOC);
    }
    
	// Data you can get: VE_FW, VE_VOLTAGE, VE_CURRENT, VE_VOLTAGE_PV, VE_POWER_PV, VE_STATE, VE_MPPT, VE_ERROR, VE_LOAD, VE_YIELD_TOTAL, VE_YIELD_TODAY, VE_POWER_MAX_TODAY, VE_YIELD_YESTERDAY, VE_POWER_MAX_YESTERDAY, VE_DAY_SEQUENCE_NUMBER, VE_LAST_LABEL, VE_SOC, VE_POWER, VE_ALARM


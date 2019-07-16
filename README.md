# VictronVEDirectArduino
Lightweight Arduino library to read data using the VE.Direct protocol from Victron Energy devices

Built as part of a larger project, now making it available separately in case others find it useful.

Setup:
 - An Arduino(ish) board 
 - A Victron Energy device that sends serial data using the text version of the VE.Direct protocol
 - A 5v to 3.3v serial converter (BMV is 3.3v - don't plug it directly into a 5v Arduino!)
 - Plugged into the Arduino on a serial port (eg Serial1, Serial2, Serial3 etc)
 - See also: https://www.victronenergy.com/live/vedirect_protocol:faq
 - Developed and tested with a BMV-700 battery monitor, should work with any that talk VE.Direct
 - Distributed under an MIT license - see LICENCE.txt

Provides:
 - Read access to VE.Direct statistics and settings
 - Defaults set to read Volts, Power, Current, State of Charge (SOC), Alarm
 - Easily extendible by adding labels for any other stats and settings of interest
 - A diagnostic "full dump" of everything coming from the device  

### Usage:
```C
#include "VEDirect.h"

VEDirect my_bmv(Serial3);
my_int32 = my_bmv.read(VE_SOC);	

// VE_SOC, VE_VOLTAGE, VE_CURRENT, VE_POWER, VE_ALARM
```


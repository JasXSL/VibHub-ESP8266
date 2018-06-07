# Work in progress

Goal is to correct the errors in rev 1.0 and make additional improvements in the case additional devboard PCBs are made (likely given how cheap that is)

## TODO

~~Swap SDA/SCL pins on ESP8266 and header~~

Replace resistor array package to fit 0402 that the EXB28V actually uses (never trust what summary descriptions show)

Move buttons, obstructed by plugin boards

Add LED to driver enable/sleep, GPIO15

Add 0805 resistor (possibly 51R or 220R) to RGB LED 3v3 to limit overall brightness as needed

Add more spacing between GND pad and pins on DRV8833 package

Consider switching USB output ports V+ through 2.5mm jack switch to allow using only one port per channel at a time

Add I2C header for great justice

Debug why auto flash and programming doesn't work

Remove auto boot mode since it may not even work and isn't required

Add instructions to slikscreen for manual flash boot mode

Add FTDI style header near relocated flash and reboot pins (GND, CTS, VCC, TXD, RXD, RTS)

Enlarged all text on silkscreen so its readable

Replace battery input with JST-XH (only needs silkscreen changes and more clearance)

Add test points, maybe

Other things
Requires wifimanager library

Tested using this
https://github.com/bbx10/WiFiManager/tree/esp32

Which requires his modified libs (until these are pulled upstream)
https://github.com/bbx10/WebServer_tng
https://github.com/bbx10/DNSServer_tng

LED indicator wiring:
Common anode of RGB LED (or discrete LEDs) to 3.3v
Red through 470ohm to RED_LED pin
Green  through 330ohm to GREEN_LED pin
Blue  through 330ohm to BLUE_LED pin
Resistor values can be lower, but this is already really bright at 2.5v for green/blue.

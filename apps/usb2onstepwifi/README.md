## This is a module that can be used with the standalone onstep focuser

### The main idea here is that the focuser by default expects that you have an onstep mount connected and in this case it allows you to use the focuser as a more standalone without an onstep mount
* The focuser is running on an ESP32 S2 chip
* The focuser is using onstep setup with JUST the focuser
* The focuser exposes a wifi access point
* Another ESP32 or ESP8266 chip is used to connect to that access point
* This code is running on the ESP32 or ESP8266 chip to act as a man in the middle to talk to the focuser
* When onstep in indi connects to the focuser, this intercepts some of the commands not in the focuser, such as the following pieces of information:
  * latitude
  * longitude
  * local time
  * sidereal time
  * date

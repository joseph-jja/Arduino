
// wifi config
const char* ssid     = "ESP-GPS-AP-25";
const char* password = "ESP8266-GPS-AP-25";

// accelerometer gyroscope address
const int MPU = 0x68; 

// gps
// GPIO 15 is ping D8
// GPIO 13 is ping D7
static const int RXPin = 13, TXPin = 15;
static const int GPSBaud = 9600;
static const signed long tzOffset = -7;

// serial baud rate
static const int SERIAL_BAUD = 9600;

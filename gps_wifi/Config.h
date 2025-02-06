
// wifi config
const char* ssid     = "ESP-GPS-AP-25";
const char* password = "ESP8266-GPS-AP-25";


// accelerometer gyroscope address
const int MPU=0x68; 

// gps
static const int RXPin = 3, TXPin = 1;
static const int GPSBaud = 9600;
static const signed long tzOffset = -7;

// serial baud rate
static const int SERIAL_BAUD = 19200;

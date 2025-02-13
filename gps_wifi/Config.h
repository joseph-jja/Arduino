
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

// conversion from C to F
// 9 / 5 = 1.8
static const float C2F_MULTIPLIER = 1.8;
static const float C2F_ADDITION = 32;

// run calculate_offsets resting to get these
// Gyro default offsets
static const float GyroOffsetX = 0.01;
static const float GyroOffsetY = 0.0;
static const float GyroOffsetZ = 0.0;

// run calculate_offsets() moving all axis to get these
// Accelerometer default offsets
static const float AccelOffsetX = -1.70;
static const float AccelOffsetY = 0.54;
static const float AccelOffsetZ = 0.00;

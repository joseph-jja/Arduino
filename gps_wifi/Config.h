
// wifi config
const char* ssid     = "ESP-GPS-AP-25";
const char* password = "ESP8266-GPS-AP-25";

// accelerometer gyroscope address
const int MPU = 0x68; 

// gps
// GPIO 2 is ping D4
// GPIO 0 is ping D3
static const int RXPin = 0, TXPin = 2;
static const int GPSBaud = 9600;
static const signed long tzOffset = -7;

// SPI for compass
static const int CLOCKPin = 14,
    MISOPin = 12,
    MOSIPin = 13,
    CSPin = 15;

// serial baud rate
static const int SERIAL_BAUD = 9600;

// conversion from C to F
// 9 / 5 = 1.8
static const float C2F_MULTIPLIER = 1.8;
static const float C2F_ADDITION = 32;

// run calculate_offsets resting to get these
// Gyro default offsets
static const float GyroOffsetX = -0.01;
static const float GyroOffsetY = 0.0;
static const float GyroOffsetZ = 0.0;

// run calculate_offsets() moving all axis to get these
// Accelerometer default offsets
static const float AccelOffsetMinX = -9.72;
static const float AccelOffsetMaxX = 9.89;
static const float AccelOffsetX = -1.17;
static const float AccelOffsetMinY = -9.89;
static const float AccelOffsetMaxY = 9.87;
static const float AccelOffsetY = 0.08;
static const float AccelOffsetMinZ = -9.80;
static const float AccelOffsetMaxZ = 9.89;
static const float AccelOffsetZ = 0.10;

// magnetomemeter data
double compassX = 0;
double compassY = 0;
double compassZ = 0;
double compassHeading = 0;

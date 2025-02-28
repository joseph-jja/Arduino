// globals all the other include files
#include "Globals.h"

// configuration
#include "Config.h"

// separate file for this lovely HTML 
#include "static_html.h"

IPAddress local_IP(192, 168, 25, 1);
IPAddress gateway(192, 168, 25, 1);
IPAddress subnet(255, 255, 255, 0);

// The TinyGPS object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);

// compass
Adafruit_LIS3MDL lis3mdl;

// mpu6050
Adafruit_MPU6050 mpu;

float AccX = 0.0,
    AccY = 0.0,
    AccZ = 0.0;
float GyroX = 0.0,
    GyroY = 0.0,
    GyroZ = 0.0;

float temperatureC = 0.0,
    temperatureF = 0.0;

char gps_date[12];
char gps_time[6];

float latitude = 0,
    longitude = 0;

void setup() {

    Serial.begin(SERIAL_BAUD);
    delay(100);
    Serial.println("Application setup!");

    Serial.print("GPS library ");
    Serial.println(TinyGPSPlus::libraryVersion());
    ss.begin(GPSBaud);

    Serial.print("Setting up Access Point");
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    /*WiFi.enableInsecureWEP(false);
    WiFi.mode(WIFI_STA);
  
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //Serial.print(".");
        Serial.println(WiFi.status());
        WiFi.printDiag(Serial);
    }
    Serial.println("");*/

    IPAddress IP = WiFi.softAPIP();
    Serial.print("WiFi with IP address: ");
    Serial.println(IP);

    // Print ESP8266 Local IP Address
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    // Route for root / web page
    //Serial.println(index_html);
    server.on("/", []() {
        Serial.println("Request for home page");
        server.send(200, "text/html", index_html);
    });
    server.on("/update", []() {
        char temp[512];
        memset(temp, '\0', sizeof(temp));
        sprintf(temp, "{ \"degC\": %2.2f, \"degF\": %2.2f, \"gps\": {\"latitude\": %3.2f, \"longitude\": %3.2f, \"date\": %s, \"time\": \"%s\"}, \"Acc\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f }, \"Gyro\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f } }",
            temperatureC, temperatureF,
            latitude, longitude,
            gps_date, gps_time,
            AccX, AccY, AccZ,
            GyroX, GyroY, GyroZ);
        Serial.print("Update request ");
        Serial.println(temp);
        server.send(200, "text/html", temp);
    });

    // Start server
    server.begin();

    // Try to initialize MPU6050!
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    setupMPU6050();

    setup_builtin_pin();
}

float get_angle(float raw_reading, float axis_max, float axis_min, float axis_resting) {
     
     if (raw_reading >= 0) {
        return (raw_reading / (axis_max - axis_resting)) * 100;
     } else {
        return (abs(raw_reading) / (abs(axis_min) - abs(axis_resting))) * -100;
     }
}

void getAccelerometerData() {

    sensors_event_t a, g, temp;
    mpu.getEvent( & a, & g, & temp);

    AccX = a.acceleration.x;
    AccY = a.acceleration.y;
    AccZ = a.acceleration.z;
    GyroX = g.gyro.x - GyroOffsetX;
    GyroY = g.gyro.y - GyroOffsetY;
    GyroZ = g.gyro.z - GyroOffsetZ;
    temperatureC = temp.temperature;
    temperatureF = (C2F_MULTIPLIER * temperatureC) + C2F_ADDITION;

    // our case the movements are slow
    // and more interested in positional angle
    // which we can use gravity for
    AccX = get_angle(AccX, AccelOffsetMaxX, AccelOffsetMinX, AccelOffsetX);
    AccY = get_angle(AccY, AccelOffsetMaxY, AccelOffsetMinY, AccelOffsetY);
    AccZ = get_angle(AccY, AccelOffsetMaxZ, AccelOffsetMinZ, AccelOffsetZ);
    
    /* Print out the values */
    Serial.print("Angle ");
    Serial.print("X: ");
    Serial.print(AccX);
    Serial.print(", Y: ");
    Serial.print(AccY);
    Serial.print(", Z: ");
    Serial.print(AccZ);
    Serial.println(" m/s^2");

    

    Serial.print("Rotation X: ");
    Serial.print(GyroX);
    Serial.print(", Y (N/S): ");
    Serial.print(GyroY);
    Serial.print(", Z (E/W): ");
    Serial.print(GyroZ);
    Serial.println(" rad/s");

    Serial.print("Chip Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" degC and ");
    Serial.print(temperatureF);
    Serial.println(" degF");

    Serial.println("");
}

void loop() {

    // uncomment this to calibrate offsets
    //calculate_offsets();

    blink_pin(1000);

    get_gps_info();
    delay(100);

    getAccelerometerData();
    delay(100);

    server.handleClient();
    MDNS.update();
}

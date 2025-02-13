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

long year = 0, month = 0, day = 0,
    currentHour = 0,
    currentMinute = 0;
float latitude = 0,
    longitude = 0,
    altitude = 0;

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

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
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
        sprintf(temp, "{ \"degC\": %2.2f, \"degF\": %2.2f, \"latitude\": %3.2f, \"longitude\": %3.2f, \"time\": \"%2.0f:%2.0f\", \"Acc\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f }, \"Gyro\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f } }",
            temperatureC, temperatureF,
            latitude, longitude,
            currentHour, currentMinute,
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

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Pin enabled");
    digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

void get_gps_info() {

    long avail = ss.available();

    while (avail > 0) {
        //Serial.println("Trying to get GPS data.");
        if (gps.encode(ss.read())) {
            blink_pin(100);
            Serial.print("GPS data read using ");
            Serial.print(gps.satellites.value());
            Serial.println(" satellites.");

            if (gps.location.isUpdated()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                Serial.print("Got valid latitude and longitude ");
                Serial.print(latitude, 6);
                Serial.print(longitude, 6);
                Serial.println("");
                delay(1000);
            }
            if (gps.altitude.isUpdated()) {
                altitude = gps.altitude.feet();
                Serial.print("Got valid altitude ");
                Serial.print(altitude, 6);
                Serial.println("");
            }
            if (gps.time.isUpdated()) {
                currentHour = gps.time.hour();
                currentMinute = gps.time.minute();
                Serial.print("Got valid time ");
                if (gps.time.hour() < 10) Serial.print(F("0"));
                Serial.print(currentHour);
                Serial.print(F(":"));
                if (gps.time.minute() < 10) Serial.print(F("0"));
                Serial.print(currentMinute);
                Serial.println("");
            }
            if (gps.date.isUpdated()) {
                year = gps.date.year();
                month = gps.date.month();
                day = gps.date.day();
                Serial.print("Got valid date ");
                Serial.print(year);
                Serial.print("/");
                Serial.print(month);
                Serial.print("/");
                Serial.print(day);
                Serial.println("");
            }
        }
        avail = ss.available();
    }
    delay(500);
}

float axis_correction(float raw_reading, float axis_offset, float grav_accel) {
    return raw_reading; //(float)(2.0 * grav_accel * (raw_reading - axis_offset)) / (axis_offset * 2);
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

    /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(AccX);
    Serial.print(" ");
    //Serial.print(axis_correction(AccX, AccelOffsetX, AccZ));
    Serial.print(" ");
    Serial.print(", Y: ");
    Serial.print(AccY);
    Serial.print(" ");
    //Serial.print(axis_correction(AccY, AccelOffsetY, AccZ));
    Serial.print(", Z: ");
    Serial.print(AccZ);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(GyroX);
    Serial.print(", Y: ");
    Serial.print(GyroY);
    Serial.print(", Z: ");
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

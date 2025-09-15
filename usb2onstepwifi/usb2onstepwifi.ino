#include <ESP8266WiFi.h>
#include <ctype.h>
#include <Wire.h>

#include "Config.h"

//#define USB_DEBUG_ENABLED 1
//#define USE_I2C_CHANNEL 1
#define ESP32_I2C_ADDRESS 24
#define WIFI_CLIENT_READ_TIMOUT 2500

const char *ssid = STATION_ID;
const char *password = STATION_PWD;

IPAddress gateway;
WiFiClient client;

// latitude and longitude defaults holder
// +xx:yyy and +zzz:abc
#define DEFAULT_LOCATION_SIZE 20
char latitude[DEFAULT_LOCATION_SIZE];
char longitude[DEFAULT_LOCATION_SIZE];

// date and time stuff
#define DEFAULT_DATE_TIME_SIZE 10
char date_str[DEFAULT_DATE_TIME_SIZE];
char local_time_str[DEFAULT_DATE_TIME_SIZE];
char time_str[DEFAULT_DATE_TIME_SIZE];

template<typename T>
void print(T line) {
#ifdef USB_DEBUG_ENABLED
  String myString = String(line);
  Serial.print(line);
#endif
}

template<typename T>
void println(T line) {
#ifdef USB_DEBUG_ENABLED
  String myString = String(line);
  Serial.println(line);
#endif
}

void setup_builtin_pin() {
  pinMode(LED_BUILTIN, OUTPUT);
  println("Pin enabled");
  digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(sleep_time);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup_i2c_wire() {
#ifdef USE_I2C_CHANNEL
  Wire.begin(ESP32_I2C_ADDRESS);
#endif
}

// Define a callback function to handle the event
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  digitalWrite(LED_BUILTIN, LOW);
}

// Define a callback function to handle the event
void onWifiConnect(const WiFiEventStationModeConnected& event) {
    for (int i = 0; i < 5; i++) {
      blink_pin(50);
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  setup_builtin_pin();

  // called but depends on ifdef
  setup_i2c_wire();

  println("");
  println("");
  print("Connecting to ");
  println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.onStationModeDisconnected(onWifiDisconnect);
  WiFi.onStationModeConnected(onWifiConnect);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    print(".");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  gateway = WiFi.gatewayIP();
  println("");
  println("WiFi connected");
  print("IP address: ");
  println(WiFi.localIP().toString());
  print("gateway address: ");
  println(gateway.toString());

  memset(latitude, '\0', DEFAULT_LOCATION_SIZE);
  memset(longitude, '\0', DEFAULT_LOCATION_SIZE);

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE, strlen(DEFAULT_LONGITUDE));

  memset(date_str, '\0', DEFAULT_DATE_TIME_SIZE);
  memset(local_time_str, '\0', DEFAULT_DATE_TIME_SIZE);
  memset(time_str, '\0', DEFAULT_DATE_TIME_SIZE);

  memcpy(date_str, "09/12/25#", strlen("09/12/25#"));
  memcpy(local_time_str, "12:12:15#", strlen("12:12:15#"));
  memcpy(time_str, "12:12:15#", strlen("12:12:15#"));

  for (int i = 0; i < 5; i++) {
    blink_pin(10);
    delay(5);
  }

  Serial.flush();
}

// compares 2 string
boolean compare(char *commandIn, char *commandStr) {
  return (memcmp(commandIn, commandStr, strlen(commandStr)) == 0);
}

// copies part of a string from source to destination
void substring(char *source, int start_index, int length, char *destination) {
  strncpy(destination, source + start_index, length);
}

/*
Set date    :SCMM/DD/YY#	Reply: 0 or 1
Get date	:GC#	Reply: MM/DD/YY#
Set time (Local)	:SLHH:MM:SS#	Reply: 0 or 1
Get time (Local, 12hr format)	:Ga#	Reply: HH:MM:SS#
Get time (Local, 24hr format)	:GL#	Reply: HH:MM:SS#
Set time (Sidereal)	:SSHH:MM:SS#	Reply: 0 or 1
Get time (Sidereal)	:GS#	Reply: HH:MM:SS#

Site/Location commands	 
Set UTC Offset(for current site)	:SGsHH#	Reply: 0 or 1
Get UTC Offset(for current site)	:GG#	Reply: sHH#
The UTC Offset value is the number of hours to add to your Local Time (Standard Time) to get Universal Time.	 
Set Latitude (for current site)	:StsDD*MM#	Reply: 0 or 1
Get Latitude (for current site)	:Gt#	Reply: sDD*MM#
Set Longitude (for current site)	:SgDDD*MM#	Reply: 0 or 1
Get Longitude (for current site)	:Gg#	Reply: DDD*MM#
*/
boolean check_override(char *bufferIn) {

  boolean override = false;

  if (bufferIn == NULL) {
    return override;
  }

  char buffer[20];
  memset(buffer, '\0', sizeof(buffer));
  int bufferInLen = strlen(bufferIn);

  if (bufferInLen == 0) {
    return override;
  }

  // commands to skip
  // date and time commands
  // latitude and lingitude commands and offset
  if (compare(bufferIn, ":Gt#")) {
    override = true;
    Serial.write(latitude);
  } else if (compare(bufferIn, ":St")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, latitude);
    Serial.write(0);
  } else if (compare(bufferIn, ":Gg#")) {
    override = true;
    Serial.write(longitude);
  } else if (compare(bufferIn, ":Sg")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, longitude);
    Serial.write(0);
  } else if (compare(bufferIn, ":GG#")) {
    override = true;
    char offsetBuff[5];
    memset(offsetBuff, '\0', 5);
    sprintf(offsetBuff, "%d", utcoffset);
    Serial.write(offsetBuff);
  } else if (compare(bufferIn, ":SG")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, buffer);
    utcoffset = atoi(buffer);
    Serial.write(0);
    // date functions
  } else if (compare(bufferIn, ":GC#")) {
    override = true;
    Serial.write(date_str);
  } else if (compare(bufferIn, ":SC")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, date_str);
    Serial.write(0);
    // local time functions
  } else if (compare(bufferIn, ":Ga#")) {
    override = true;
    Serial.write(local_time_str);
  } else if (compare(bufferIn, ":GL#")) {
    override = true;
    Serial.write(local_time_str);
  } else if (compare(bufferIn, ":SL")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, local_time_str);
    Serial.write(0);
    // local time functions
  } else if (compare(bufferIn, ":GS#")) {
    override = true;
    Serial.write(time_str);
  } else if (compare(bufferIn, ":SS")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, time_str);
    Serial.write(0);
  }

  print("Command ");
  print(bufferIn);
  print(" override ");
  println(override);

  // we are overriding so we make sure to flush
  if (override) {
    Serial.flush();
  }

  return override;
}

uint16_t port = 9999;
void connect_client() {

  String host_str = gateway.toString();
  const char *host = host_str.c_str();

  print("connecting to ");
  print(host_str);
  print(" ");
  print(host);
  print(":");
  println(port);

  if (!client.connect(host, port)) {
    println("connection failed");
    blink_pin(10);
    if (port >= 9996) {
      port--;
    } else {
      port = 9999;
    }
    return;
  } else {
    println("connected!!");
    client.keepAlive(86400, 100, 10000);
    client.setTimeout(86400);
    client.setNoDelay(true);
    blink_pin(10);
  }
}

void reconnect_check() {
  // Use WiFiClient class to create TCP connections
  // so if there is no data to read on the port the client.connected() 
  // returns false or closed, which is inaccurate
  // status can be established which means we are connected
  boolean isConnected = client.connected();
  boolean isEstablishedConnection = (client.status() == ESTABLISHED);
  if (!isConnected) {
    client.stop();
    connect_client();
    delay(10);
  }
}

char bufferIn[128];
char bufferOut[128];

// generic function for reading data into bufferIn from USB port
boolean read_in_usb_data() {

  boolean isCommandOverridden = false;

  memset(bufferIn, '\0', sizeof(bufferIn));

  int i = 0;
  /*
    a sentace is a command starting with : and ending with #
    we need a whole sentance in order for the commands to be recognized
  */
  boolean sentance = true;
  boolean capture = false;
  while (Serial.available() || sentance) {
    char incomingByte = Serial.read();
    if (incomingByte != NULL && isprint(incomingByte)) {
      print("USB data being read ");
      println(incomingByte);
      if (incomingByte == ':') {
        capture = true;
      }
      if (capture) {
        bufferIn[i] = incomingByte;
        i++;
      }
      if (incomingByte == '#') {
        capture = false;
        sentance = false;
      }
    }
  }

  Serial.flush();
  return check_override(bufferIn);
}

void read_in_wifi_data() {

  reconnect_check();
  
  memset(bufferOut, '\0', sizeof(bufferOut));
  int j = 0;
  int start_time = millis();
  print("Checking data?");
  println(client.available());
  while (millis() - start_time < WIFI_CLIENT_READ_TIMOUT) {
    if (client.available()) {
      char incomingByte = client.read();
      print("WIFI data read in ");
      println(incomingByte);
      if (incomingByte != NULL && isprint(incomingByte)) {
        bufferOut[j] = incomingByte;
        j++;
      }
    }
    delay(10);
  }

  int end = strlen(bufferOut);
  if (end > 0) {
    boolean valid = true;
    Serial.write(bufferOut);
    Serial.flush();
    print("We responded with ");
    println(bufferOut);
  }
}

void read_in_wire_data(boolean isCommandOverridden) {

  memset(bufferOut, '\0', sizeof(bufferOut));
  int j = 0;
  while (Wire.available()) {
    char incomingByte = Wire.requestFrom(ESP32_I2C_ADDRESS, 1);
    if (incomingByte != NULL && isprint(incomingByte)) {
      bufferOut[j] = incomingByte;
      j++;
    }
  }

  if (strlen(bufferOut) > 0 && !isCommandOverridden) {
    Serial.write(bufferOut);
    Serial.flush();
    print("We responded with ");
    println(bufferOut);
  }
}

void use_wifi_client() {

  reconnect_check();

  // read in the data from USB port
  boolean isCommandOverridden = read_in_usb_data();
  if (!isCommandOverridden) {
    print("Sending focuser the command ");
    println(bufferIn);
    client.write(bufferIn);
    client.flush();
    delay(10);

    // try a few times to read in data
    // a sort of polling
    read_in_wifi_data();
  }
}

void use_wire_client() {
  // read in the data from USB port
  boolean isCommandOverridden = read_in_usb_data();
  if (!isCommandOverridden) {
    Wire.write(bufferIn);
  }

  read_in_wire_data(isCommandOverridden);
}

void loop() {

#ifdef USE_I2C_CHANNEL
  use_wire_client();
#else
  use_wifi_client();
  // to flush or not to flush
  Serial.flush();
  client.flush();
#endif
}

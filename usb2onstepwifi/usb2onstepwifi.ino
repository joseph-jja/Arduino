#include <ESP8266WiFi.h>
#include <ctype.h>
#include <Wire.h>

#include "Config.h"
#include "Constants.h"
#include "string_functions.h"
#include "Overrides.h"

const char *ssid = STATION_ID;
const char *password = STATION_PWD;

IPAddress gateway;
WiFiClient client;

// latitude and longitude defaults holder
// +xx:yyy and +zzz:abc
char latitude[DEFAULT_LOCATION_SIZE];
char longitude[DEFAULT_LOCATION_SIZE];
int utcoffset = 5;

// date and time stuff
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
void onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
  digitalWrite(LED_BUILTIN, LOW);
  println("WiFi disconnected!");
}

// Define a callback function to handle the event
void onWifiConnect(const WiFiEventStationModeConnected &event) {
  for (int i = 0; i < 5; i++) {
    blink_pin(50);
  }
  println("WiFi connected!");
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

  overrides.init();

  for (int i = 0; i < 5; i++) {
    blink_pin(50);
    delay(25);
  }

  Serial.flush();
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

void write_out_usb_data(char *buffer) {

    Serial.write(buffer);
    Serial.flush();
    print("We responded with ");
    println(buffer);
}

// generic function for reading data into bufferIn from USB port
bool read_in_usb_data(char usbBufferIn[], char usbBufferOut[]) {

  memset(usbBufferIn, '\0', sizeof(usbBufferIn));
  memset(usbBufferOut, '\0', sizeof(usbBufferOut));

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
        usbBufferIn[i] = incomingByte;
        i++;
      }
      if (incomingByte == '#') {
        capture = false;
        sentance = false;
      }
    }
  }

  return overrides.check_override(usbBufferIn, usbBufferOut, 128);
}

void write_out_wifi_data(char *buffer) {

    client.write(buffer);
    client.flush();
    print("Sending focuser the command ");
    println(buffer);
}

void read_in_wifi_data(char wifiBufferOut[], char usbBufferIn[]) {

  memset(wifiBufferOut, '\0', sizeof(wifiBufferOut));

  bool isBinaryReply = boolean_reply(usbBufferIn);
  
  char endings[2];
  ends_with(usbBufferIn, endings);

  int j = 0;
  int start_time = millis();
  print("Checking data?");
  println(client.available());
  reconnect_check();
  while ((millis() - start_time) < WIFI_CLIENT_READ_TIMOUT) {
    if (client.available()) {
      char incomingByte = client.read();
      print("WIFI data read in ");
      println(incomingByte);
      if (incomingByte != NULL && isprint(incomingByte)) {
        wifiBufferOut[j] = incomingByte;
        j++;
      }
    }
    delay(10);
  }
}

void use_wifi_client() {

  reconnect_check();

  char usbBufferIn[128];
  char usbBufferOut[128];
  char wifiBufferOut[128];

  // read in the data from USB port
  boolean isCommandOverridden = read_in_usb_data(usbBufferIn, usbBufferOut);
  if (isCommandOverridden && strlen(usbBufferOut) > 0) {
    write_out_usb_data(usbBufferOut);
    print("Command ");
    print(usbBufferIn);
    print(" override ");
    println(isCommandOverridden);
  } else {
    write_out_wifi_data(usbBufferIn);

    // check if we should be getting a response
    boolean hasResponse = has_reply(usbBufferIn);
    if (!hasResponse) {
        print("Message in has no reply data ");
        println(usbBufferIn);
        return;
    }
    // try a few times to read in data
    // a sort of polling
    read_in_wifi_data(wifiBufferOut, usbBufferIn);
    if (strlen(wifiBufferOut) > 0) {
        write_out_usb_data(wifiBufferOut);
    }
  }
}

void read_in_wire_data(boolean isCommandOverridden) {

  /*memset(bufferOut, '\0', sizeof(bufferOut));
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
  }*/
}

void use_wire_client() {
  // read in the data from USB port
  /*boolean isCommandOverridden = read_in_usb_data();
  if (!isCommandOverridden) {
    Wire.write(bufferIn);
  }

  read_in_wire_data(isCommandOverridden);*/
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

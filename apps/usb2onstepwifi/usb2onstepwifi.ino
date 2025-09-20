#include <ESP8266WiFi.h>
#include <ctype.h>
#include <Wire.h>

#include "Config.h"
#include "Constants.h"
#include "string_functions.h"
#include "Overrides.h"

#define BUFFER_SIZE 128

const char *ssid = STATION_ID;
const char *password = STATION_PWD;

IPAddress gateway;
WiFiClient client;

// latitude and longitude defaults holder
// +xx:yyy and +zzz:abc
char latitude[DEFAULT_LOCATION_SIZE];
char longitude[DEFAULT_LOCATION_SIZE];
int utcoffset = 5;

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

  overrides.init(millis());

  for (int i = 0; i < 5; i++) {
    blink_pin(50);
    delay(25);
  }

  Serial.flush();
}

uint16_t port = 9999;
bool connect_client() {

  String host_str = gateway.toString();
  const char *host = host_str.c_str();

  int i = 0;
  bool isConnected = false;
  while (!isConnected && i < 10) {
    isConnected = client.connect(host, port);
    if (!isConnected) {
      println("Cnnection failed");
      digitalWrite(LED_BUILTIN, LOW);
      if (port > 9996) {
        port--;
      } else {
        port = 9999;
      }
    }
    i++;
  }
  print("Connected? ");
  print(isConnected);
  print(" to ");
  print(host_str);
  print(" on port ");
  println(port);
}

void write_out_usb_data(char *buffer) {

    Serial.write(buffer);
    delay(10);
    Serial.flush();
    print("We responded with ");
    println(buffer);
}

bool cbp = 0;

// generic function for reading data into bufferIn from USB port
bool read_in_usb_data(char usbBufferIn[], char usbBufferOut[]) {

  memset(usbBufferIn, '\0', BUFFER_SIZE);
  memset(usbBufferOut, '\0', BUFFER_SIZE);

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
      // special for lx200 protocol
      if (!capture && cbp == 0 && incomingByte == '6') {
        cbp = 2;
        usbBufferIn[0] = 'A';
        sentance = false;
      }
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

  return overrides.check_override(usbBufferIn, usbBufferOut, BUFFER_SIZE, millis());
}

void write_out_wifi_data(char *buffer) {

    client.write(buffer);
    delay(10);
    client.flush();
    print("Sending focuser the command ");
    println(buffer);
}

void read_in_wifi_data(char wifiBufferOut[], char usbBufferIn[]) {

  memset(wifiBufferOut, '\0', BUFFER_SIZE);

  bool isBinaryReply = boolean_reply(usbBufferIn);
  
  char endings[2];
  ends_with(usbBufferIn, endings);

  int j = 0;
  int start_time = millis();
  bool foundEnd = false;
  println("Trying to read in data!");
  while ((millis() - start_time) < WIFI_CLIENT_READ_TIMOUT && !foundEnd) {
    if (client.available()) {
      char incomingByte = client.read();
      print("WIFI data read in ");
      println(incomingByte);
      if (incomingByte != NULL && isprint(incomingByte)) {
        wifiBufferOut[j] = incomingByte;
          // then before delay
          if (isBinaryReply && (incomingByte == '0' || incomingByte == '1')) {
            foundEnd = true;
          } else if (endings != NULL && endings[0] == incomingByte) {
            foundEnd = true;
          }
        j++;
      }
    }
    delay(25);
  }
  print("Read time took");
  println(millis() - start_time);
  println(wifiBufferOut);
  println(endings);
}

void use_wifi_client() {

  char usbBufferIn[BUFFER_SIZE];
  char usbBufferOut[BUFFER_SIZE];
  char wifiBufferOut[BUFFER_SIZE];

  // read in the data from USB port
  boolean isCommandOverridden = read_in_usb_data(usbBufferIn, usbBufferOut);
  if (isCommandOverridden && strlen(usbBufferOut) > 0) {
    if (usbBufferOut == USB_RESET_REPLY) {
      cbp = 0;
    }
    write_out_usb_data(usbBufferOut);
    print("Command ");
    print(usbBufferIn);
    print(" override ");
    println(isCommandOverridden);
  } else if (cbp == 2) {
    // special case for lx200 
    cbp = 3;
    write_out_usb_data(usbBufferOut);  
  } else {
    bool isConnected = connect_client();
    if (isConnected) {
      delay(10);
      write_out_wifi_data(usbBufferIn);

      // check if we should be getting a response
      boolean hasResponse = has_reply(usbBufferIn);
      if (!hasResponse) {
          print("Message in has no reply data ");
          println(usbBufferIn);
          delay(10);
          client.stop();
          return;
      }
      // try a few times to read in data
      // a sort of polling
      read_in_wifi_data(wifiBufferOut, usbBufferIn);
      if (strlen(wifiBufferOut) > 0) {
          write_out_usb_data(wifiBufferOut);
      }
      delay(10);
      client.stop();
    }
  }
}

void loop() {
  use_wifi_client();
}

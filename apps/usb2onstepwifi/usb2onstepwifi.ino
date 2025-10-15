#include <ESP8266WiFi.h>
#include <ctype.h>
#include <Wire.h>

#include "Config.h"
#include "Constants.h"
#include "string_functions.h"

#define BUFFER_SIZE 128

const char *ssid = STATION_ID;
const char *password = STATION_PWD;
IPAddress gateway;
WiFiClient client;

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
  Serial.flush();
  print("We responded with ");
  println(buffer);
}

int ack_command_state = 0;

// generic function for reading data into bufferIn from USB port
void read_in_usb_data(char usbBufferIn[]) {

  memset(usbBufferIn, '\0', BUFFER_SIZE);

  /*
    a sentace is a command starting with : and ending with #
    we need a whole sentance in order for the commands to be recognized
    we need to wait to get an actual command
  */
  bool sentance = true;
  bool capture = false;
  bool timedOut = false;

  int start_time = millis();
  int i = 0;
  while (!timedOut && sentance) {
    if (Serial.available()) {
      char incomingByte = Serial.read();
      if (incomingByte != NULL && (incomingByte == (char)6 || isprint(incomingByte))) {

        print("USB data being read ");
        print(ack_command_state);
        print(" ");
        print((int)incomingByte);
        print(" ");
        println(incomingByte);

        // special for lx200 protocol
        if (!capture && ack_command_state == 0 && incomingByte == (char)6) {
          ack_command_state = 2;
          usbBufferIn[0] = incomingByte;
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
    timedOut = ((millis() - start_time) > USB_READ_TIMOUT);
  }
  delay(25);
}

void write_out_wifi_data(char *buffer) {

  client.write(buffer);
  client.flush();
  print("Sending focuser the command ");
  println(buffer);
  delay(50);
}

void read_in_wifi_data(char wifiBufferOut[], char usbBufferIn[]) {

  memset(wifiBufferOut, '\0', BUFFER_SIZE);

  // check if we are expecting a binary reply
  bool isBinaryReply = boolean_reply(usbBufferIn);

  // check if the command ends with # or *
  char endings[2];
  ends_with(usbBufferIn, endings);

  int j = 0;
  int start_time = millis();
  bool foundEnd = false;
  bool timedOut = false;
  println("Trying to read in data!");
  while (!foundEnd && !timedOut) {
      if (client.available()) {
        char incomingByte = client.read();
        print("WIFI data read in ");
        println(incomingByte);
        if (incomingByte != NULL && isprint(incomingByte)) {
          wifiBufferOut[j] = incomingByte;
          // then before delay
          if (isBinaryReply && (incomingByte == '0' || incomingByte == '1')) {
            foundEnd = true;
          } else if (!isNull(endings) && endings[0] == incomingByte) {
            foundEnd = true;
          }
          j++;
        }
      }
      timedOut = ((millis() - start_time) > WIFI_CLIENT_READ_TIMOUT);
  }
  print("Read time took");
  println(millis() - start_time);
  println(wifiBufferOut);
  println(endings);
  delay(10);
}

long last_loop = millis();
void loop() {

  char usbBufferIn[BUFFER_SIZE];
  char wifiBufferOut[BUFFER_SIZE];

  // read in the data from USB port
  read_in_usb_data(usbBufferIn);

  if (ack_command_state == 2 && !isNull(usbBufferIn)
    && usbBufferIn[0] == (char)6) {
    ack_command_state = 0;
    write_out_usb_data(MOUNT_MODE);
  } else if (!isNull(usbBufferIn)) {
    // got data in the buffer
    bool isConnected = connect_client();
    if (isConnected) {
      write_out_wifi_data(usbBufferIn);

      // check if we should be getting a response
      boolean hasResponse = has_reply(usbBufferIn);
      if (hasResponse) {
        // try a few times to read in data
        // a sort of polling
        read_in_wifi_data(wifiBufferOut, usbBufferIn);
        if (!isNull(wifiBufferOut)) {
          write_out_usb_data(wifiBufferOut);
        }
      } else {
        print("Message in has no reply data ");
        println(usbBufferIn);
      }
      client.stop();
    }
  }
  delay(50);
}

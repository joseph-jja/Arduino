#include <ESP8266WiFi.h>

#define STATION_ID "xxx"
#define STATION_PWD "xxx"

const char* ssid     = STATION_ID;
const char* password = STATION_PWD;

IPAddress gateway;
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  gateway = WiFi.gatewayIP();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
  Serial.print("gateway address: ");
  Serial.println(gateway.toString());
}

void loop() {
  
   String host_str = gateway.toString();
   const char* host = host_str.c_str();
   uint16_t port = 9999;

    // Use WiFiClient class to create TCP connections
    if (!client.connected()) {
      Serial.print("connecting to ");
      Serial.print(host_str);
      Serial.print(' ');
      Serial.print(host);
      Serial.print(':');
      Serial.println(port);

      if (!client.connect(host, port)) {
        Serial.println("connection failed");
        delay(5000);
        if (port >= 9996) {
          port--;
        } else {
          port = 9999;
        } 
        return;
      } else {
        Serial.println("connected!!");
        client.keepAlive(86400, 100, 100);
      }
    }
    delay(10);

    if (client.connected()) {

      while (Serial.available()) {
        Serial.println("Data being read");
        client.write(Serial.read());
      }

      while (client.available()) {
        Serial.write(client.read());
      }

      client.flush();
    }
}

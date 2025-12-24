// configuration
#include "wifi_server_config.h"

IPAddress local_IP(192, 168, 25, 1);
IPAddress gateway(192, 168, 25, 1);
IPAddress subnet(255, 255, 255, 0);

// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);

void setup_wifi_server() {

  Serial.begin(9600);
  delay(100);

  Serial.print("Setting up Access Point");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

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
    /*updateData();
    char *responseHTML;
    responseHTML = (char *)malloc(strlen(jsonData) + strlen(index_html) + 1);
    if (responseHTML) {
      memset(responseHTML, '\0', sizeof(responseHTML));
      sprintf(responseHTML, index_html, jsonData);
      Serial.print("Request for home page");
      Serial.println(strlen(responseHTML));
      server.send(200, "text/html", responseHTML);
      free(responseHTML);
    } else {*/
    server.send(200, "text/html", "Memory alloc failed");
    //}
  });

  // Start server
  server.begin();
}
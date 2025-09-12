#include <ESP8266WiFi.h>

#include <Config.h>

//#define USB_DEBUG_ENABLED  

const char* ssid     = STATION_ID;
const char* password = STATION_PWD;

IPAddress gateway;
WiFiClient client;

// latitude and longitude defaults holder
// +xx:yyy and +zzz:abc
static int DEFAULT_LOCATION_SIZE = 20;
char latitude[DEFAULT_LOCATION_SIZE];
char longitude[DEFAULT_LOCATION_SIZE];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

#ifdef USB_DEBUG_ENABLED
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef USB_DEBUG_ENABLED
    Serial.print(".");
#endif
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  gateway = WiFi.gatewayIP();
#ifdef USB_DEBUG_ENABLED
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
  Serial.print("gateway address: ");
  Serial.println(gateway.toString());
#endif

  memset(latitude, '\0', sizeof(latitude)); 
  memset(longitude, '\0', sizeof(longitude)); 

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE , strlen(DEFAULT_LONGITUDE));

  Serial.flush();
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
boolean check_override(command) {

   boolean overide = false;

   // commands to skip
   // date and time commands
   // latitude and lingitude commands and offset
   if (memcmp(bufferIn, ":Gt#", strlen(":Gt#")) {
     overide = true;
     Serial.write(latitude);
   } else if (memcmp(bufferIn, ":Gg#", strlen(":Gg#")) {
     overide = true;
     Serial.write(longitude);
   } else if (memcmp(bufferIn, ":GG#", strlen(":GG#")) {
     overide = true;
     Serial.write(utcoffset);
   }
   return override;
}

void loop() {
  
   String host_str = gateway.toString();
   const char* host = host_str.c_str();
   uint16_t port = 9999;

    // Use WiFiClient class to create TCP connections
    if (!client.connected()) {
#ifdef USB_DEBUG_ENABLED
      Serial.print("connecting to ");
      Serial.print(host_str);
      Serial.print(' ');
      Serial.print(host);
      Serial.print(':');
      Serial.println(port);
#endif

      if (!client.connect(host, port)) {
#ifdef USB_DEBUG_ENABLED
        Serial.println("connection failed");
#endif
        delay(5000);
        if (port >= 9996) {
          port--;
        } else {
          port = 9999;
        } 
        return;
      } else {
#ifdef USB_DEBUG_ENABLED
        Serial.println("connected!!");
#endif
        client.keepAlive(86400, 100, 100);
      }
    }
    delay(10);

    if (client.connected()) {

      char bufferIn[128];
      char bufferOut[128];

      memset(bufferIn, '\0', sizeof(bufferIn)); 
      memset(bufferOut, '\0', sizeof(bufferOut)); 

      int i = 0;
      while (Serial.available()) {
#ifdef USB_DEBUG_ENABLED
        Serial.println("Data being read ");
#endif
        char incomingByte = Serial.read();
        if (incomingByte != -1 && incomingByte != NULL) {
          bufferIn[i] = incomingByte;
          i++;
        }
      }

      boolean isCommandOverridden = false;
      if (strlen(bufferIn) > 0) {
#ifdef USB_DEBUG_ENABLED
        Serial.print("We got the command in ");
        Serial.println(bufferIn);
#endif
        isCommandOverridden = check_override(bufferIn);
        if (!isCommandOverridden) {
            client.write(bufferIn);
        }
      }

      int j = 0;
      while (client.available()) {
        char incomingByte = client.read();
        if (incomingByte != -1 && incomingByte != NULL) {
          bufferOut[j] = incomingByte;
          j++;
        }
      }

      if (strlen(bufferOut) > 0 && !isCommandOverridden) {
        Serial.write(bufferOut);
#ifdef USB_DEBUG_ENABLED
        Serial.print("We responded with ");
        Serial.println(bufferOut);
#endif
      }

      Serial.flush();
      client.flush();
    }
}

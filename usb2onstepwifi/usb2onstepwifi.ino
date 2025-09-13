#include <ESP8266WiFi.h>
#include <ctype.h>

#include "Config.h"

//#define USB_DEBUG_ENABLED 1 

const char* ssid     = STATION_ID;
const char* password = STATION_PWD;

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

void setup_builtin_pin() {
    pinMode(LED_BUILTIN, OUTPUT);
    //Serial.println("Pin enabled");
    digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

void print(char *line) {
#ifdef USB_DEBUG_ENABLED
  Serial.print(line);
#endif
}

void println(char *line) {
#ifdef USB_DEBUG_ENABLED
  Serial.println(line);
#endif
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  setup_builtin_pin();

  println("");
  println("");
  print("Connecting to ");
  println((char *)ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

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
  println((char *)WiFi.localIP().toString().c_str());
  print("gateway address: ");
  println((char *)gateway.toString().c_str());

  memset(latitude, '\0', DEFAULT_LOCATION_SIZE); 
  memset(longitude, '\0', DEFAULT_LOCATION_SIZE); 

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE , strlen(DEFAULT_LONGITUDE));

  memset(date_str, '\0', DEFAULT_DATE_TIME_SIZE); 
  memset(local_time_str, '\0', DEFAULT_DATE_TIME_SIZE); 
  memset(time_str, '\0', DEFAULT_DATE_TIME_SIZE); 

  memcpy(date_str, "09/12/25#", strlen("09/12/25#"));
  memcpy(local_time_str, "12:12:15#" , strlen("12:12:15#"));
  memcpy(time_str, "12:12:15#" , strlen("12:12:15#"));

  for ( int i =0; i < 5; i++) {
    blink_pin(10);
    delay(5);
  }

  Serial.flush();
}

boolean compare(char *commandIn, char *commandStr) {
    return (memcmp(commandIn, commandStr, strlen(commandStr)) == 0);
}

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

    char buffer[20];
    memset(buffer, '\0', sizeof(buffer));
    int bufferInLen = strlen(bufferIn);

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
  println((char *)String(override).c_str());

   return override;
}

void loop() {
  
   String host_str = gateway.toString();
   const char* host = host_str.c_str();
   uint16_t port = 9999;

    // Use WiFiClient class to create TCP connections
    if (!client.connected()) {
      print("connecting to ");
      print((char *)host_str.c_str());
      print(" ");
      print((char *)host);
      print(":");
      println((char *)String(port).c_str());

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
        client.keepAlive(86400, 100, 100);
        blink_pin(10);
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
        println("Data being read ");
        char incomingByte = Serial.read();
        if (incomingByte != NULL && isprint(incomingByte)) {
          bufferIn[i] = incomingByte;
          i++;
        }
      }

      boolean isCommandOverridden = false;
      if (strlen(bufferIn) > 0) {
        isCommandOverridden = check_override(bufferIn);
        print("We got the command in ");
        println(bufferIn);
        print("Override says what ");
        println((char *)String(isCommandOverridden).c_str());
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
        print("We responded with ");
        println(bufferOut);
      }

      Serial.flush();
      client.flush();
    }
}

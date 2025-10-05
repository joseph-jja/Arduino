#pragma once

// wifi change these
const char* ssid     = "****************";
const char* password = "***********";
IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// define this to either SERVER_MODE or CLIENT_MODE
// defining both results in error
//#define CLIENT_MODE 1
#define SERVER_MODE 1

// client default update interval is 15m
#define UPDATE_INTERVAL  900000

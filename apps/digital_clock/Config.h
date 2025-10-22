#pragma once

// wifi change these
const char* ssid     = "****************";
const char* password = "***********";
IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// client default update interval is 15m
#define UPDATE_INTERVAL  900000

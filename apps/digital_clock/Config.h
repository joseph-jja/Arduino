// wifi change these
const char* ssid     = "****************";
const char* password = "***********";
IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

#define CLIENT_MODE 0
#define SERVER_MODE 1

// define this to either SERVER_MODE or CLIENT_MODE
#define RUNNING_MODE SERVER_MODE

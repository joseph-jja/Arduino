#pragma once

#define USB_RESET_REPLY "USB Reset"
#define USB_RESET_COMMAND ":USB_RESET#"

#define ACK_COMMAND_IN ":ACK#"

class Overrides {
public:
  // Public members (accessible from outside the class)
  bool check_override(char *bufferIn, char returnOut[], int buffer_size, unsigned long ms);
};

extern Overrides overrides;

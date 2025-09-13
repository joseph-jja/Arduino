// USB2I2C plugin
#pragma once

#include <Wire.h>

#include "../../lib/commands/CommandErrors.h"

// Define the I2C address for this device
#define I2C_SLAVE_ADDRESS 0x24

class USB2I2C {
public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

private:

};

extern USB2I2C usb2i2c;

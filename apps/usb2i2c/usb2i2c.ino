// Sample plugin
#include "usb2i2c.h"
#include "../../Common.h"
#include "../../lib/serial/Serial_Local.h"
#include "../../lib/tasks/OnTask.h"

void usb2i2cWrapper() { usb2i2c.loop(); }

// This function is called when the master writes to this device
void receiveEvent(int byteCount) {
  // Loop through all incoming bytes
  while (Wire.available()) {
    char c = Wire.read();
    SERIAL_LOCAL.print(c);
  }
  SERIAL_LOCAL.println();
}

void USB2I2C::init() {
  VLF("MSG: Plugins, starting: usb2i2c");

  // start a task that runs twice a second, run at priority level 7 so
  // we can block using tasks.yield(); fairly aggressively without significant impact on operation
  tasks.add(500, 0, true, 7, usb2i2cWrapper);

  // Initialize the I2C interface as a client (slave)
  // The first argument is the client's address, the second is the SDA pin, and the third is the SCL pin.
  // By default, the ESP32-S2 uses GPIO 8 for SDA and GPIO 9 for SCL.
  Wire.begin(I2C_SLAVE_ADDRESS);
}

void USB2I2C::loop() {

}

USB2I2C usb2i2c;

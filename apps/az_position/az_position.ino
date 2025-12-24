#include "LIS3MDL_config.h"
#include "sdd1306_config.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // we don't need this
  //Wire.begin(21, 22);

  setup_SDD1306();
  clear_display();
  setup_magnetometer();

  rotate(1);
}

void loop() {
  // put your main code here, to run repeatedly:

  loop_magnetometer();
  MAGNETOMETER results = getLIS3MDLReadings();

  clear_display();

  write_display(2, 2, "Compass");
  write_number(6, 12, results.compassHeading);

  write_display(2, 26, "Magnetic");
  write_number(6, 36, results.magneticHeading); 

  show();

  delay(1000);
}

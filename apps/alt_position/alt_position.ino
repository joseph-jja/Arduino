#include "mpu6050_config.h"
#include "sdd1306_config.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  setup_SDD1306();
  setup_accel_n_gyro();

  rotate(3);
}

void loop() {
  // put your main code here, to run repeatedly:

  loop_accel_n_gyro();
  MPU6050_PROCESSED_DATA results = getMPU6050Data();

  clear();

  write_display(2, 2, "A Roll");
  write_number(6, 12, results.accAngleX);

  write_display(2, 22, "A Pitch");
  write_number(6, 32, results.accAngleY);

  write_display(2, 42, "G Roll");
  write_number(6, 52, results.gyroRoll);

  write_display(2, 62, "G Pitch");
  write_number(6, 72, results.gyroPitch);

  write_display(2, 82, "F Roll");
  write_number(6, 92, results.filteredRoll);

  write_display(2, 102, "F Pitch");
  write_number(6, 112, results.filteredPitch);
  

  show();

  delay(1000);
}

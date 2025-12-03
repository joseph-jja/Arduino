#import "mpu6050_config.h"
#import "sdd1306_config.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  setup_SDD1306();
  setup_accel_n_gyro();
}

void loop() {
  // put your main code here, to run repeatedly:

  loop_accel_n_gyro();
  MPU6050_PROCESSED_DATA results = getMPU6050Data();

  clear_display();

  char buff[32];
  memset(buff, '\0', 32);
  sprintf(buff, "Acc Roll %0.02f", results.accAngleX);
  write_display(5, 5, buff);

  memset(buff, '\0', 32);
  sprintf(buff, "Acc Roll %0.02f", results.accAngleY);
  write_display(5, 5, buff);

  delay(1000);
}

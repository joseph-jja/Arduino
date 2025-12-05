#include "LIS3MDL_config.h"

// compass
Adafruit_LIS3MDL lis3mdl;
MAGNETOMETER magneto;

void setup_magnetometer() {

  // Try to initialize!
  // for 5 seconds
  int i = 0;
  boolean found = lis3mdl.begin_I2C(LSL3MDL_ADDRESS, &Wire);
  while (!found && i < 500) {  // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Failed to find LIS3MDL chip, will try again in 10");
    delay(10);
    found = lis3mdl.begin_I2C(LSL3MDL_ADDRESS, &Wire);
  }
  if (!found) {
    Serial.println("LIS3MDL NOT Found, stopping!");
    return;
  }

  Serial.println("LIS3MDL Found!");

  // we want high performance
  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  // good quality vs speed
  lis3mdl.setDataRate(LIS3MDL_DATARATE_40_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }

  // highly sensitive
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  // need this to work continually
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true,  // enable z axis
                          true,                // polarity
                          false,               // don't latch
                          true);               // enabled!
}

void loop_magnetometer() {

  lis3mdl.read();  // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("Compass readings: ");
  magneto.compassX = lis3mdl.x;
  magneto.compassY = lis3mdl.y;
  magneto.compassZ = lis3mdl.z;
  Serial.print("X:  ");
  Serial.print(magneto.compassX);
  Serial.print("  \tY:  ");
  Serial.print(magneto.compassY);
  Serial.print("  \tZ:  ");
  Serial.println(magneto.compassZ);

  /* Or....get a new sensor event, normalized to uTesla */
  sensors_event_t event;
  lis3mdl.getEvent(&event);
  magneto.magneticX = event.magnetic.x;
  magneto.magneticY = event.magnetic.y;
  magneto.magneticZ = event.magnetic.z;
  /* Display the results (magnetic field is measured in uTesla) */
  Serial.print("Magnetic readings: ");
  Serial.print("\tX: ");
  Serial.print(magneto.magneticX);
  Serial.print(" \tY: ");
  Serial.print(magneto.magneticY);
  Serial.print(" \tZ: ");
  Serial.print(magneto.magneticZ);
  Serial.println(" uTesla ");

  magneto.compassHeading = atan2(magneto.compassY, magneto.compassX) * RAD_TO_DEG;
  magneto.magneticHeading = atan2(magneto.magneticY, magneto.magneticX) * RAD_TO_DEG;

  Serial.print("Headings: ");
  Serial.print("Compass: ");
  Serial.print(magneto.compassHeading);
  Serial.print(" Magnetic: ");
  Serial.print(magneto.magneticHeading);
  Serial.println("");
}

MAGNETOMETER getLIS3MDLReadings() {
  return magneto;
}

/* usage example */
/*
void setup() {

  Serial.begin(9600);
  setup_magnetometer();
}

void loop() {
  loop_magnetometer();
  MAGNETOMETER myMPU = getLIS3MDLReadings();
  // do something with the data, like display on screen?
  delay(1000);
}*/

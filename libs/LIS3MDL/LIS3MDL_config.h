#pragma once

#include <math.h>

// compass
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>

typedef struct {
  // updated
  bool updated;

  // conspass
  float compassX;
  float compassY;
  float compassZ;

   // magnetometer
  float magneticX;
  float magneticY;
  float magneticZ;

  float compassHeading;
  float magneticHeading;
} MAGNETOMETER;

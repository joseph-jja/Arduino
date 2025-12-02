#pragma once

// accelerometer gyroscope address
const int MPU = 0x68; 

#define FILTER_COEFFICIENT 0.98
static const float DELTA_FILTER_COEFFICIENT = 1 - FILTER_COEFFICIENT;

typedef struct {
  // updated
  bool updated;

  // accelerometer and gyroscope
  float AccX;
  float AccY;
  float AccZ;
  float GyroX;
  float GyroY;
  float GyroZ;

  // temperature
  float temperatureC;
  float temperatureF;

  // roll and pitch 
  double accRoll;
  double accPitch; 
  double gyroRoll;
  double filteredRoll; 
  double filteredPitch; 

} MPU6050_PROCESSED_DATA;

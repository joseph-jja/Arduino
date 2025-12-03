#pragma once

#include <math.h>

// accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// accelerometer gyroscope address
const int MPU = 0x68; 

#define FILTER_COEFFICIENT 0.98
static const float DELTA_FILTER_COEFFICIENT = 1 - FILTER_COEFFICIENT;

#define ACCEL_OFFSET_X 1.2
#define ACCEL_OFFSET_Y 0.1
#define ACCEL_OFFSET_Z 0.12

#define AVERAGE_LOOP_COUNT 5

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

  // accelerometer angles
  float accAngleX;
  float accAngleY;

  // roll and pitch rates 
  double gyroRollRate;
  double gyroPitchRate;

  double gyroRoll;
  double gyroPitch;

  double filteredRoll; 
  double filteredPitch; 

} MPU6050_PROCESSED_DATA;

#pragma once

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
} MPU6050_data;

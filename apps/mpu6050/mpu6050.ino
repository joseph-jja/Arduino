#import "mpu6050_config.h"

// conversion from C to F
// 9 / 5 = 1.8
static const float C2F_MULTIPLIER = 1.8;
static const float C2F_ADDITION = 32;

// mpu6050
Adafruit_MPU6050 mpu;

MPU6050_data mpudata;

void setup_accel_n_gyro() {

  // Try to initialize MPU6050!
  int i = 0;
  if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (mpu.begin() && i < 50) {
          delay(100);
      }
  }

  // we want to detect smaller movements so we use 2G
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
        case MPU6050_RANGE_2_G:
            Serial.println("+-2G");
            break;
        case MPU6050_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case MPU6050_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case MPU6050_RANGE_16_G:
            Serial.println("+-16G");
            break;
    }
    
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
        case MPU6050_RANGE_250_DEG:
            Serial.println("+- 250 deg/s");
            break;
        case MPU6050_RANGE_500_DEG:
            Serial.println("+- 500 deg/s");
            break;
        case MPU6050_RANGE_1000_DEG:
            Serial.println("+- 1000 deg/s");
            break;
        case MPU6050_RANGE_2000_DEG:
            Serial.println("+- 2000 deg/s");
            break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
        case MPU6050_BAND_260_HZ:
            Serial.println("260 Hz");
            break;
        case MPU6050_BAND_184_HZ:
            Serial.println("184 Hz");
            break;
        case MPU6050_BAND_94_HZ:
            Serial.println("94 Hz");
            break;
        case MPU6050_BAND_44_HZ:
            Serial.println("44 Hz");
            break;
        case MPU6050_BAND_21_HZ:
            Serial.println("21 Hz");
            break;
        case MPU6050_BAND_10_HZ:
            Serial.println("10 Hz");
            break;
        case MPU6050_BAND_5_HZ:
            Serial.println("5 Hz");
            break;
    }
}

void loop_accel_n_gyro() {

    sensors_event_t a, g, temp;
    mpu.getEvent( & a, & g, & temp);

    mpudata.AccX = a.acceleration.x;
    mpudata.AccY = a.acceleration.y;
    mpudata.AccZ = a.acceleration.z;
    mpudata.GyroX = g.gyro.x;
    mpudata.GyroY = g.gyro.y;
    mpudata.GyroZ = g.gyro.z;
    mpudata.temperatureC = temp.temperature;
    mpudata.temperatureF = (C2F_MULTIPLIER * mpudata.temperatureC) + C2F_ADDITION;
    
    /* Print out the values */
    Serial.print("RAW Angle ");
    Serial.print("X: ");
    Serial.print(mpudata.AccX);
    Serial.print(", Y: ");
    Serial.print(mpudata.AccY);
    Serial.print(", Z: ");
    Serial.print(mpudata.AccZ);
    Serial.println(" m/s^2");

    

    Serial.print("RAW Gyro X: ");
    Serial.print(mpudata.GyroX);
    Serial.print(", Y (N/S): ");
    Serial.print(mpudata.GyroY);
    Serial.print(", Z (E/W): ");
    Serial.print(mpudata.GyroZ);
    Serial.println(" rad/s");

    Serial.print("Chip Temperature: ");
    Serial.print(mpudata.temperatureC);
    Serial.print(" degC and ");
    Serial.print(mpudata.temperatureF);
    Serial.println(" degF");

    Serial.println("");
}

// TODO create get method to get data values

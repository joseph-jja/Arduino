// accel and gyro code from 
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
float midx = 0.0, amidx = 0.0;
float midy = 0.0, amidy = 0.0;
float midz = 0.0, amidz = 0.0;

// Gyroscope calibration needs to be sitting still
// Accelerometer calibrate under graviation for each axis
// look on chip for dot in the corner as that identifies x
// on my board long way is y, short way is x and up and down is z
// https://github.com/Blokkendoos/mpu-calibration
// update these after calibration
// update Config.h with calibration values
/*
12:46:52.775 -> Gyro Calibration Results (Min/Max/Mid)
12:46:52.808 -> X : -0.03/0.00/-0.01
12:46:52.808 -> Y : -0.03/0.03/-0.00
12:46:52.841 -> Z : -0.00/0.00/0.00

14:38:38.080 -> Accelerometer Calibration Results (Min/Max/Mid)
12:48:48.967 -> X : 0.00/10.05/5.03
12:54:21.765 -> Y : 0.00/10.49/5.25
12:46:52.973 -> Z : 0.00/10.18/5.09

13:02:31.158 -> X : -10.11/0.00/-5.06
13:03:34.756 -> Y : -10.30/0.00/-5.15
12:56:33.568 -> Z : -12.46/0.00/-6.23

*/

float x, y, z, ax, ay, az;
float minx = 0.0, maxx = 0.0;
float miny = 0.0, maxy = 0.0;
float minz = 0.0, maxz = 0.0;
float aminx = 0.0, amaxx = 0.0;
float aminy = 0.0, amaxy = 0.0;
float aminz = 0.0, amaxz = 0.0;

void calculate_offsets() {

    Serial.println("Running Accelerometer and Gyro Calbration");

    for (int i = 0; i < 1000; i++) {
        // calibrate gyro
        sensors_event_t a, g, temp;
        mpu.getEvent( & a, & g, & temp);

        x = g.gyro.x;
        y = g.gyro.y;
        z = g.gyro.z;

        // gyro
        minx = min(x, minx);
        maxx = max(x, maxx);
        midx = (maxx + minx) / 2;

        miny = min(y, miny);
        maxy = max(y, maxy);
        midy = (maxy + miny) / 2;

        minz = min(z, minz);
        maxz = max(z, maxz);
        midz = (maxz + minz) / 2;

        ax = a.acceleration.x;
        ay = a.acceleration.y;
        az = a.acceleration.z;

        // accel
        aminx = min(ax, aminx);
        amaxx = max(ax, amaxx);
        amidx = (aminx + amaxx) / 2;

        aminy = min(ay, aminy);
        amaxy = max(ay, amaxy);
        amidy = (aminy + amaxy) / 2;

        aminz = min(az, aminz);
        amaxz = max(az, amaxz);
        amidz = (aminz + amaxz) / 2;

        delay(10);
    }

    Serial.println("Gyro Calibration Results (Min/Max/Mid)");
    Serial.print("X : ");
    Serial.print(minx);
    Serial.print("/");
    Serial.print(maxx);
    Serial.print("/");
    Serial.print(midx);
    Serial.println("");
    Serial.print("Y : ");
    Serial.print(miny);
    Serial.print("/");
    Serial.print(maxy);
    Serial.print("/");
    Serial.print(midy);
    Serial.println("");
    Serial.print("Z : ");
    Serial.print(minz);
    Serial.print("/");
    Serial.print(maxz);
    Serial.print("/");
    Serial.print(midz);
    Serial.println("");
    Serial.println("Accelerometer Calibration Results (Min/Max/Mid)");
    Serial.print("X : ");
    Serial.print(aminx);
    Serial.print("/");
    Serial.print(amaxx);
    Serial.print("/");
    Serial.print(amidx);
    Serial.println("");
    Serial.print("Y : ");
    Serial.print(aminy);
    Serial.print("/");
    Serial.print(amaxy);
    Serial.print("/");
    Serial.print(amidy);
    Serial.println("");
    Serial.print("Z : ");
    Serial.print(aminz);
    Serial.print("/");
    Serial.print(amaxz);
    Serial.print("/");
    Serial.print(amidz);
    Serial.println("");
}

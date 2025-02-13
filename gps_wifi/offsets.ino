// accel and gyro code from 
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
float midx = 0.0, amidx = 0.0;
float midy = 0.0, amidy = 0.0;
float midz = 0.0, amidz = 0.0;

// this seems to work by getting run twice
// the first time is resting on a table for the gyro calibration
// the second is moving each acccess for the accelerometer calibration
// output looks like this
// using the last value seems to work
// update config.h values
/*
16:09:09.692 -> Gyro Calibration Results (Min/Max/Mid)
16:09:09.726 -> X : -0.02/0.00/-0.01
16:09:09.726 -> Y : -0.01/0.01/-0.00
16:09:09.759 -> Z : -0.00/0.00/0.00
16:13:20.660 -> Accelerometer Calibration Results (Min/Max/Mid)
16:13:20.693 -> X : -17.65/14.24/-1.70
16:13:20.726 -> Y : -11.67/12.75/0.54
16:13:20.759 -> Z : 0.00/0.00/0.00
*/
void calculate_offsets() {

    Serial.println("Running Accelerometer and Gyro Calbration");

    float x, y, z, ax, ay, az;
    float minx = 0.0, maxx = 0.0;
    float miny = 0.0, maxy = 0.0;
    float minz = 0.0, maxz = 0.0;
    float aminx = 0.0, amaxx = 0.0;
    float aminy = 0.0, amaxy = 0.0;
    float aminz = 0.0, amaxz = 0.0;

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
// configuration
#include "Globals.h"

// D0
int interruptPin = 16;

volatile bool newDataAvailable = true;

void setup_mmc5983ma() {

    //Wire.begin();

    //pinMode(interruptPin, INPUT);
    //attachInterrupt(digitalPinToInterrupt(interruptPin), interruptRoutine, RISING);

    /*if (myMag.begin() == false) {
        Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
        //while (true);
        return;
    }*/

    //myMag.softReset();

    Serial.println("MMC5983MA connected");
    /*
    Serial.println("Setting filter bandwith to 100 Hz for continuous operation...");
    myMag.setFilterBandwidth(100);
    Serial.print("Reading back filter bandwith: ");
    Serial.println(myMag.getFilterBandwith());

    Serial.println("Setting continuous measurement frequency to 10 Hz...");
    myMag.setContinuousModeFrequency(10);
    Serial.print("Reading back continuous measurement frequency: ");
    Serial.println(myMag.getContinuousModeFrequency());

    Serial.println("Enabling auto set/reset...");
    myMag.enableAutomaticSetReset();
    Serial.print("Reading back automatic set/reset: ");
    Serial.println(myMag.isAutomaticSetResetEnabled() ? "enabled" : "disabled");

    Serial.println("Enabling continuous mode...");
    myMag.enableContinuousMode();
    Serial.print("Reading back continuous mode: ");
    Serial.println(myMag.isContinuousModeEnabled() ? "enabled" : "disabled");

    Serial.println("Enabling interrupt...");
    //myMag.enableInterrupt();
    Serial.print("Reading back interrupt status: ");
    Serial.println(myMag.isInterruptEnabled() ? "enabled" : "disabled");

    // Set our interrupt flag, just in case we missed the rising edge
    newDataAvailable = true;*/
}
/*
void get_compass_data() {
    
    if (newDataAvailable == true) {

        uint32_t rawValueX = 0;
        uint32_t rawValueY = 0;
        uint32_t rawValueZ = 0;

        newDataAvailable = false; // Clear our interrupt flag
        myMag.clearMeasDoneInterrupt(); // Clear the MMC5983 interrupt

        // Read all three channels simultaneously
        // Note: we are calling readFieldsXYZ to read the fields, not getMeasurementXYZ
        // The measurement is already complete, we do not need to start a new one
        myMag.readFieldsXYZ(&rawValueX, &rawValueY, &rawValueZ);
    
        // The magnetic field values are 18-bit unsigned. The _approximate_ zero (mid) point is 2^17 (131072).
        // Here we scale each field to +/- 1.0 to make it easier to calculate an approximate heading.
        //
        // Please note: to properly correct and calibrate the X, Y and Z channels, you need to determine true
        // offsets (zero points) and scale factors (gains) for all three channels. Futher details can be found at:
        // https://thecavepearlproject.org/2015/05/22/calibrating-any-compass-or-accelerometer-for-arduino/
        compassX = (double)rawValueX - 131072.0;
        compassX /= 131072.0;
    
        compassY = (double)rawValueY - 131072.0;
        compassY /= 131072.0;
    
        compassZ = (double)rawValueZ - 131072.0;
        compassZ /= 131072.0;
    
        // Magnetic north is oriented with the Y axis
        // Convert the X and Y fields into heading using atan2 (Arc Tangent 2)
        compassHeading = atan2(compassX, 0 - compassY);
    
        // atan2 returns a value between +PI and -PI
        // Convert to degrees
        compassHeading /= PI;
        compassHeading *= 180;
        compassHeading += 180;
    
        Serial.print("Heading: ");
        Serial.println(compassHeading, 1);
    }
}
*/
void interruptRoutine() {
    newDataAvailable = true;
}

// configuration
#include "Globals.h"

void setup_builtin_pin() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Pin enabled");
    digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

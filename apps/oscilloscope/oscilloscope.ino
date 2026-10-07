// Define analog input pin connected to the voltage divider output
const int analogInPin = 4; // Change to your preferred ADC1 GPIO pin

// Calibration multipliers for 62k and 10k resistor divider
const float resistorRatio = (62.0 + 10.0) / 10.0; 
const float adcMaxVoltage = 3.3; // ESP32 ADC reference voltage
const int adcResolution = 4095;  // 12-bit ADC

void setup() {
  Serial.begin(115200);
  // Set ADC attenuation for 0-3.3V range
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int rawValue = analogRead(analogInPin);
  
  // Convert raw reading to pin voltage (0 - 3.3V)
  float pinVoltage = (rawValue / (float)adcResolution) * adcMaxVoltage;
  
  // Scale back up to actual 24V input level
  float actualVoltage = pinVoltage * resistorRatio;
  
  // Print for Arduino Serial Plotter
  Serial.print("Voltage:");
  Serial.println(actualVoltage);
  
  delay(10); // Adjust sampling interval as needed
}

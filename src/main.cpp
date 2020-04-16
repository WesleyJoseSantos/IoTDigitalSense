#include "main.h"

IoTDigitalSense iotSense;

void setup() {
  Serial.begin(74880);
  Serial.println("IoT Digital Sense Started.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  iotSense.task();
}
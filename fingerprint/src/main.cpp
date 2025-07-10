#include <Arduino.h>
#include "enroll/fingerprintSensor.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nESP32 Fingerprint Enrollment");
  setupFingerprintSensor();
}

void loop() {
  Serial.println("\nReady to enroll a new fingerprint!");
  uint8_t id = readNumber();
  if (id == 0) {
    Serial.println("Invalid ID. Use a number from 1 to 127.");
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (!enrollFingerprint(id));
}

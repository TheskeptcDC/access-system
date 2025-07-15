#include "DetectFinger.h"
#include <HardwareSerial.h>
#include "sensor/FingerprintSensor.h"

// GPIO pin to trigger
#define FINGER_TRIGGER_PIN 13

void setupFingerDetection() {
  pinMode(FINGER_TRIGGER_PIN, OUTPUT);
  digitalWrite(FINGER_TRIGGER_PIN, LOW);
  delay(100);

  if (finger.verifyPassword()) {
    Serial.println("✅ Fingerprint sensor ready (detect module)");
  } else {
    Serial.println("❌ Could not find fingerprint sensor (detect module)");
    while (true) delay(1);
  }
}

void detectFingerAndTrigger() {
  uint8_t p = finger.getImage();

  if (p == FINGERPRINT_OK) {
    Serial.println("👉 Finger detected (detect module) → HIGH");
    digitalWrite(FINGER_TRIGGER_PIN, HIGH);
  } else if (p == FINGERPRINT_NOFINGER) {
    Serial.println("✋ No finger → LOW");
    digitalWrite(FINGER_TRIGGER_PIN, LOW);
  } else {
    Serial.print("⚠️ Sensor error: ");
    Serial.println(p);
    digitalWrite(FINGER_TRIGGER_PIN, LOW);
  }
}

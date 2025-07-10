#include "../sensor/FingerprintSensor.h"
#include "Search.h"

uint8_t searchFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return p;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return p;

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence ");
    Serial.println(finger.confidence);
  } else {
    Serial.println("Fingerprint not recognized.");
  }

  return p;
}

int searchFingerprintFast() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;

  Serial.print("Fast Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}

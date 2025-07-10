#include "../sensor/FingerprintSensor.h"
#include "Enroll.h"

uint8_t readNumber() {
  uint8_t num = 0;
  Serial.println("Enter ID (1-127):");
  while (num == 0) {
    while (!Serial.available());
    num = Serial.parseInt();
  }
  while (Serial.available()) Serial.read();
  return num;
}

uint8_t enrollFingerprint(uint8_t id) {
  int p = -1;

  Serial.print("Place finger to enroll as ID #");
  Serial.println(id);
  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    if (p == FINGERPRINT_NOFINGER) Serial.print(".");
    else return p;
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK) return p;

  Serial.println("Remove finger...");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER);

  Serial.println("Place same finger again...");
  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    if (p == FINGERPRINT_NOFINGER) Serial.print(".");
    else return p;
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK) return p;

  if (finger.createModel() != FINGERPRINT_OK) return p;
  if (finger.storeModel(id) != FINGERPRINT_OK) return p;

  Serial.println("Enrollment complete.");
  return true;
}

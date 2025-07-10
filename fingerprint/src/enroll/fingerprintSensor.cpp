#include "FingerprintSensor.h"

HardwareSerial mySerial(2); // UART2
Adafruit_Fingerprint finger(&mySerial);

void setupFingerprintSensor() {
  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) delay(1);
  }

  Serial.println(F("Reading sensor parameters..."));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
}

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
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) {
      Serial.print(".");
    } else if (p == FINGERPRINT_OK) {
      Serial.println("Image taken");
    } else {
      Serial.println("Error capturing image.");
      return p;
    }
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("Failed to convert image to char file 1.");
    return p;
  }

  Serial.println("Remove finger...");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER);

  Serial.println("Place same finger again...");
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) Serial.print(".");
    else if (p == FINGERPRINT_OK) Serial.println("Second image taken");
    else return p;
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("Failed to convert second image.");
    return p;
  }

  if (finger.createModel() != FINGERPRINT_OK) {
    Serial.println("Failed to create model. Fingers did not match?");
    return p;
  }

  if (finger.storeModel(id) == FINGERPRINT_OK) {
    Serial.println("Fingerprint enrolled successfully!");
    return true;
  } else {
    Serial.println("Failed to store fingerprint.");
    return p;
  }
}

#include "FingerprintSensor.h"

HardwareSerial mySerial(2); // UART2
Adafruit_Fingerprint finger(&mySerial);

void setupFingerprintSensor() {
  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor.");
    while (1) delay(1);
  }

  finger.getParameters();
  finger.getTemplateCount();

  Serial.print("Templates stored: ");
  Serial.println(finger.templateCount);
}

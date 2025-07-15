#include <Arduino.h>
#include "sensor/FingerprintSensor.h"
#include "enroll/Enroll.h"
#include "search/Search.h"
#include "search/detectFinger.h"
#define CAMERA_PIN 13 //pin for camera trigger

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("ESP32 Fingerprint System");
  pinMode(CAMERA_PIN, OUTPUT);
  digitalWrite(CAMERA_PIN, LOW);
  setupFingerprintSensor();
   setupFingerDetection();   
}

void loop() {
  // Serial.println("\nChoose Option:\n1 - Enroll\n2 - Search");
  // while (!Serial.available());
  // char option = Serial.read();
  // while (Serial.available()) Serial.read();

  // if (option == '1') {
    // uint8_t id = readNumber();
    // enrollFingerprint(id);
  // } else if (option == '2') {
    // searchFingerprint();
  // }

detectFingerAndTrigger();
  delay(2000);
}

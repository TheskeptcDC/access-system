#ifndef FINGERPRINT_SENSOR_H
#define FINGERPRINT_SENSOR_H

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

extern HardwareSerial mySerial;
extern Adafruit_Fingerprint finger;

void setupFingerprintSensor();

#endif

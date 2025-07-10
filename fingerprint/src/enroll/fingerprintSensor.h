#ifndef FINGERPRINT_SENSOR_H
#define FINGERPRINT_SENSOR_H

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// UART pins and instance
#define RX_PIN 16
#define TX_PIN 17
extern HardwareSerial mySerial;
extern Adafruit_Fingerprint finger;

void setupFingerprintSensor();
uint8_t readNumber();
uint8_t enrollFingerprint(uint8_t id);

#endif

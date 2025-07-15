#ifndef DETECT_FINGER_H
#define DETECT_FINGER_H

#include <Adafruit_Fingerprint.h>

// Call in setup to initialize sensor and output pin
void setupFingerDetection();

// Call this in loop to detect finger and trigger pin
void detectFingerAndTrigger();

#endif

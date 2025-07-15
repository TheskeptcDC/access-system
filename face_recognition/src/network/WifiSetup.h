#pragma once
#include <Arduino.h>

void connectToWiFi(const char* ssid, const char* password);
String getDeviceID();           // returns MAC with colons replaced
IPAddress getLocalIP();

#include "WiFiSetup.h"
#include <WiFi.h>

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nCamera connected to network");
    Serial.println(WiFi.localIP());
}

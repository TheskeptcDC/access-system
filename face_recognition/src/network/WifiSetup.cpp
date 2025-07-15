#include "WiFiSetup.h"
#include <WiFi.h>

static String deviceID;

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi connected");
    Serial.print("📶 IP address: ");
    Serial.println(WiFi.localIP());

    deviceID = WiFi.macAddress();
    deviceID.replace(":", "_");  // clean MAC for topic use
}

String getDeviceID() {
    return deviceID;
}

IPAddress getLocalIP() {
    return WiFi.localIP();
}

#include "MqttClient.h"
#include <WiFi.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setupMQTT(const char* server, int port) {
    mqttClient.setServer(server, port);
    mqttClient.setBufferSize(60056);
    mqttClient.setKeepAlive(300);
}

void ensureMQTTConnected(const char* clientId, const char* user, const char* password) {
    while (!mqttClient.connected()) {
        Serial.print("camera attempting connection with broker...");
        if (mqttClient.connect(clientId, user, password)) {
            Serial.println("camera connected to broker");
        } else {
            Serial.print("failed, rc=");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}

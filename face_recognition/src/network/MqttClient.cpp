#include "MqttClient.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String mqttImageTopic;
String mqttEndTopic;

void setupMQTT(const char* server, int port, const char* baseTopic, const char* endBaseTopic, const String& deviceID) {
    mqttClient.setServer(server, port);
    mqttClient.setBufferSize(60056);
    mqttClient.setKeepAlive(300);

    mqttImageTopic = String(baseTopic) + "/" + deviceID;
    mqttEndTopic = String(endBaseTopic) + "/" + deviceID;

    Serial.print("📡 MQTT image topic: ");
    Serial.println(mqttImageTopic);
    Serial.print("📡 MQTT end topic: ");
    Serial.println(mqttEndTopic);
}

void ensureMQTTConnected(const char* clientId, const char* user, const char* password) {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect(clientId, user, password)) {
            Serial.println("✅ MQTT connected");
        } else {
            Serial.print("❌ failed, rc=");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}

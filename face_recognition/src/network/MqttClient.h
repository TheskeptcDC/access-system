#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>

extern PubSubClient mqttClient;
extern String mqttImageTopic;
extern String mqttEndTopic;

void setupMQTT(const char* server, int port, const char* baseTopic, const char* endBaseTopic, const String& deviceID);
void ensureMQTTConnected(const char* clientId, const char* user, const char* password);

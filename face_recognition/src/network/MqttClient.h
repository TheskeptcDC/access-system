#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>

extern PubSubClient mqttClient;
void setupMQTT(const char* server, int port);
void ensureMQTTConnected(const char* clientId, const char* user = "", const char* password = "");

#endif

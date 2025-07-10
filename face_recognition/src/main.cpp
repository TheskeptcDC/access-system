#include <Arduino.h>
#include "camera/CameraSetup.h"
#include "network/WiFiSetup.h"
#include "network/MqttClient.h"
#include "storage/SDStorage.h"
#include "config/config.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

const char *mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_user = MQTT_USER;
const char *mqtt_password = MQTT_PASSWORD;
const char *mqtt_topic = "esp32cam/image";
const char *mqtt_end_topic = "esp32cam/image_end";

void setup() {
    Serial.begin(115200);

    setupCamera();
    connectToWiFi(ssid, password);
    setupMQTT(mqtt_server, mqtt_port);
    initSDCard();
}

void loop() {
    ensureMQTTConnected("ESP32Camera", mqtt_user, mqtt_password);
    mqttClient.loop();

    camera_fb_t *fb = captureImage();
    if (fb && saveImageToSD(fb)) {
        publishImageFromSD(mqttClient, mqtt_topic, mqtt_end_topic);
    }

    delay(5000); // Delay between captures
}

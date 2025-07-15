#include <Arduino.h>
#include "camera/CameraSetup.h"
#include "network/WiFiSetup.h"
#include "network/MqttClient.h"
#include "storage/SDStorage.h"
#include "config/config.h" 

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const char* base_image_topic = "esp32cam/image";
const char* base_end_topic = "esp32cam/image_end";

void setup() {
    Serial.begin(115200);
    setupCamera();
    connectToWiFi(ssid, password);
    
    String deviceID = getDeviceID();
    setupMQTT(mqtt_server, mqtt_port, base_image_topic, base_end_topic, deviceID);

    initSDCard();
}

void loop() {
    ensureMQTTConnected(("ESP32CAM_" + getDeviceID()).c_str(), mqtt_user, mqtt_password);
    mqttClient.loop();

    camera_fb_t* fb = captureImage();
    if (fb && saveImageToSD(fb)) {
        publishImageFromSD(mqttClient, mqttImageTopic, mqttEndTopic);
    }

    delay(1000);
}

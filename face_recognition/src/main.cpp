#include <Arduino.h>
#include <WiFi.h>
#include <esp_camera.h>
#include <PubSubClient.h>
#include <SD_MMC.h>

// WiFi credentials
const char *ssid = "TECNO SPARK 10 Pro";
const char *password = "12345678";

// MQTT server details
const char *mqtt_server = "192.168.74.92";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_password = "";
const char *mqtt_topic = "esp32cam/image";
const char *mqtt_end_topic = "esp32cam/image_end";  // Topic for indicating end of transfer

WiFiClient espClient;
PubSubClient client(espClient);

// SD Card Image Path
#define IMAGE_PATH "/image.jpg"
#define CHUNK_SIZE 60000

// Camera configuration
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sccb_sda = SIOD_GPIO_NUM,
    .pin_sccb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 10000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_UXGA,
    .jpeg_quality = 5,
    .fb_count = 1
};

void setup_camera() {
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        Serial.println(WiFi.localIP());

        return;
    }
    delay(500);
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
}

void setup_wifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Camera", mqtt_user, mqtt_password)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void captureAndSaveImage() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed!");
        return;
    }

    File file = SD_MMC.open(IMAGE_PATH, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing!");
        return;
    }

    file.write(fb->buf, fb->len);
    file.close();
    esp_camera_fb_return(fb);

    Serial.println("Image saved to SD card!");
}

void publishImage() {
    File file = SD_MMC.open(IMAGE_PATH);
    if (!file) {
        Serial.println("Failed to open file for reading!");
        return;
    }

    size_t imageSize = file.size();
    uint8_t *buffer = (uint8_t *)malloc(CHUNK_SIZE);
    if (!buffer) {
        Serial.println("Memory allocation failed!");
        file.close();
        return;
    }

    Serial.println("Publishing image in chunks...");
    while (file.available()) {
        int bytesRead = file.read(buffer, CHUNK_SIZE);
        if (!client.publish(mqtt_topic, buffer, bytesRead)) {
            Serial.println("Chunk publish failed!");
            break;
        }
        delay(50); // Allow MQTT processing
    }

    // After publishing all the chunks, send an "end of transfer" message
    if (client.publish(mqtt_end_topic, "IMAGE_TRANSFER_COMPLETE")) {
        Serial.println("End of image transfer message sent!");
    } else {
        Serial.println("Failed to send end of transfer message!");
    }

    free(buffer);
    file.close();
    Serial.println("Image transmission complete!");
}

void setup() {
    Serial.begin(115200);
    setup_camera();
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setBufferSize(60056);  // Increase buffer size to 1024 bytes
    client.setKeepAlive(300);

    if (!SD_MMC.begin()) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized!");
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    captureAndSaveImage(); // Save image first
    delay(20);
    publishImage();        // Publish from SD card
    delay(50);
}
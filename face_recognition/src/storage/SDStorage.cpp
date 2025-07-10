#include <esp_camera.h>
#include "SDStorage.h"

bool initSDCard() {
    if (!SD_MMC.begin()) {
        Serial.println("SD Card initialization failed!");
        return false;
    }
    Serial.println("SD Card initialized.");
    return true;
}

bool saveImageToSD(camera_fb_t *fb) {
    File file = SD_MMC.open(IMAGE_PATH, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing!");
        return false;
    }
    file.write(fb->buf, fb->len);
    file.close();
    esp_camera_fb_return(fb);
    Serial.println("Image saved to SD card!");
    return true;
}

void publishImageFromSD(PubSubClient &client, const char* topic, const char* endTopic) {
    File file = SD_MMC.open(IMAGE_PATH);
    if (!file) {
        Serial.println("Failed to open file for reading!");
        return;
    }

    uint8_t *buffer = (uint8_t *)malloc(CHUNK_SIZE);
    if (!buffer) {
        Serial.println("Memory allocation failed!");
        file.close();
        return;
    }

    Serial.println("Publishing image in chunks...");
    while (file.available()) {
        int bytesRead = file.read(buffer, CHUNK_SIZE);
        if (!client.publish(topic, buffer, bytesRead)) {
            Serial.println("Chunk publish failed!");
            break;
        }
        delay(50);
    }

    if (client.publish(endTopic, "IMAGE_TRANSFER_COMPLETE")) {
        Serial.println("End of image transfer message sent!");
    }

    free(buffer);
    file.close();
    Serial.println("Image transmission complete!");
}

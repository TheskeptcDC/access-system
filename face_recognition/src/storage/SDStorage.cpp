#include "SDStorage.h"
#include "esp_camera.h"

#define IMAGE_PATH "/image.jpg"
#define CHUNK_SIZE 60000

bool initSDCard() {
    if (!SD_MMC.begin()) {
        Serial.println("❌ SD Card init failed!");
        return false;
    }
    Serial.println("✅ SD Card initialized");
    return true;
}

bool saveImageToSD(camera_fb_t* fb) {
    File file = SD_MMC.open(IMAGE_PATH, FILE_WRITE);
    if (!file) {
        Serial.println("❌ Failed to open file!");
        return false;
    }

    file.write(fb->buf, fb->len);
    file.close();
    esp_camera_fb_return(fb);

    Serial.println("✅ Image saved");
    return true;
}

void publishImageFromSD(PubSubClient& client, const String& imageTopic, const String& endTopic) {
    File file = SD_MMC.open(IMAGE_PATH);
    if (!file) {
        Serial.println("❌ Could not open image file!");
        return;
    }

    size_t imageSize = file.size();
    uint8_t* buffer = (uint8_t*)malloc(CHUNK_SIZE);
    if (!buffer) {
        Serial.println("❌ Memory allocation failed!");
        file.close();
        return;
    }

    Serial.println("📤 Sending image...");
    while (file.available()) {
        int bytesRead = file.read(buffer, CHUNK_SIZE);
        if (!client.publish(imageTopic.c_str(), buffer, bytesRead)) {
            Serial.println("❌ Publish failed!");
            break;
        }
        delay(50);
    }

    if (client.publish(endTopic.c_str(), "IMAGE_TRANSFER_COMPLETE")) {
        Serial.println("✅ Image end message sent");
    }

    free(buffer);
    file.close();
    Serial.println("✅ Done sending image");
}

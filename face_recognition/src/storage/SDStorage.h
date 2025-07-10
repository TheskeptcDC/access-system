#ifndef SD_STORAGE_H
#define SD_STORAGE_H

#include <FS.h>
#include <SD_MMC.h>
#include <PubSubClient.h>
#include <esp_camera.h>


#define IMAGE_PATH "/image.jpg"
#define CHUNK_SIZE 60000

bool initSDCard();
bool saveImageToSD(camera_fb_t *fb);
void publishImageFromSD(PubSubClient &client, const char* topic, const char* endTopic);

#endif

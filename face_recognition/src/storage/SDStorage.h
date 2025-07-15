#pragma once
#include <FS.h>
#include <SD_MMC.h>
#include <PubSubClient.h>
#include "esp_camera.h"

bool initSDCard();
bool saveImageToSD(camera_fb_t* fb);
void publishImageFromSD(PubSubClient& client, const String& imageTopic, const String& endTopic);

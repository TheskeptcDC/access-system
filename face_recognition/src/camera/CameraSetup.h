#pragma once
#include <esp_camera.h>

void setupCamera();
camera_fb_t* captureImage();

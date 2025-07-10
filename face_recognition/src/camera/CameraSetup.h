#ifndef CAMERA_SETUP_H
#define CAMERA_SETUP_H

#include <esp_camera.h>

void setupCamera();
camera_fb_t* captureImage();

#endif

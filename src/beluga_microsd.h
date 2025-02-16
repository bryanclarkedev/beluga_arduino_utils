#if 1
#pragma once

// MicroSD Libraries

#include "FS.h"
#include "SD_MMC.h"
#include "esp_camera.h" //For frame buffer save-to-pointer


/*
#define FILE_SIZE_MB 1
#define FILE_SIZE (1000000UL*FILE_SIZE_MB)
#define BUF_SIZE 100

uint8_t buf[BUF_SIZE];
*/

//Copying from https://dronebotworkshop.com/esp32-cam-microsd/

namespace beluga_utils
{
    class microsd
    {
        public:
            microsd(){};
            bool initialise();
            bool get_status();
            bool readFile(const std::string path);
            bool save_esp32_cam_image_to_microsd(std::string path_to_save, camera_fb_t  * fb);

        private:

            bool _microsd_found = false;

    };
}

#endif
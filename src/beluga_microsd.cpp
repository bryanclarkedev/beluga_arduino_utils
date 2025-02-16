
#if 0
#include "beluga_microsd.h"
#include <sstream>
namespace beluga_utils
{
    

bool microsd::initialise() {
  // Start the MicroSD card
 
  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    Serial.println("MicroSD Card Mount Failed");
    return false;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return false;
  }
  _microsd_found = true;
  return true;
 
}


bool microsd::get_status()
{
    return _microsd_found;
}

bool microsd::save_esp32_cam_image_to_microsd(std::string path_to_save, camera_fb_t  * fb)
{
  std::stringstream ss;
 // Save picture to microSD card
  fs::FS &fs = SD_MMC;
  File file = fs.open(path_to_save.c_str(), FILE_WRITE);
  if (!file) {
    ss << "Failed to open file " << path_to_save << " in write mode";
    Serial.println(ss.str().c_str());
    return false;
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path_to_save.c_str());
  }
  // Close the file
    file.close();

    return true;
}

//Copied from https://randomnerdtutorials.com/esp32-microsd-card-arduino/
bool readFile(const std::string path){
  fs::FS &fs = SD_MMC;

  Serial.printf("Reading file: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_READ); //ADded file_Read
  if(!file){
    Serial.println("Failed to open file for reading");
    return false;
  }

/*
  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  */
  file.close();

  return true;
}

}

#endif
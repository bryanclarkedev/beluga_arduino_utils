#include <Arduino.h>
//#include "beluga_device.h"

//beluga_core::device this_device;
#include <sstream>

std::string config_file_path = "/test.ini";

std::stringstream ss;
#include "beluga_debug.h"
#include "beluga_ini_reader.h"

beluga_utils::ini_reader this_ini(config_file_path);

int iter = 0;

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < 5; i++)
  {
    Serial.println(5-i);
    delay(1000);
  }    
  this_ini.initialise();
  //this_device.initialise(config_file_path, "demo_device");

  this_ini.print_config_to_serial();

  std::string val;
  bool ok = this_ini.get_config_value("demo_device", "placeholder", &val, false);
  if(! ok)
  {
   beluga_utils::debug_print("error getting data");
  }else{
    std::stringstream ss;
    ss << "Got val demo_device.placeholder = " << val;
    beluga_utils::debug_print(ss.str());
  }

  this_ini.clear();


}

void loop() {
  //bool b = this_device.run();
  //assert(b == false);
  ss.str("");
  ss << "Iteration " << iter << " time " << (int) (millis() / 1000) << "s";
  beluga_utils::debug_print(ss.str().c_str());
  iter++;
  delay(1000);
}

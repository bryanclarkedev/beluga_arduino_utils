#pragma once
#include "SPIFFS.h"
#include <sstream>
#include "beluga_string.h"
#include <vector>
#include "beluga_debug.h"
namespace beluga_utils
{
    /*!
    \brief Ini-format file reader
    \author Bryan Clarke
    \date 19/10/2024
    \todo Test with LittleFS as well as SPIFFS. Possibly replace, possibly add an option to choose between the two.
    \details Configuration data is to be read from a .ini file which is uploaded to the data partition
    on the microcontroller and managed using SPIFFS. The filename is specified in the source and must start with ./ e.g. "./config.ini". 
    This is a fast and dirty ini-file reader. If you want a more full-featured version, one option is
    to use SPIFFSIniFile: https://github.com/yurilopes/SPIFFSIniFile
    NOTE: SPIFFSIniFile uses the GPLv3 license!
    NOTE: SPIFFS uses the Apache 2.0 license!
    Every object should be configured there using the following format:
    [device_name]
    key1 = val1
    key2 = val2
    e.g.
    [my_device]
    enable_serial_log =  true
    -----
    The config section [my_device] is used to identify the device, and MUST be unique.
    Maximum length of any line is 255 characters. This includes the key, the val, the equals sign and any spaces.
    (This is defined by the _line_buffer_len variable in beluga_ini_reader.h and you could change it if you really needed to).
    Config fields are returned as strings; it is assumed that you know what the underlying data type really is and
    can manually perform type conversions.
    -----
    Usage:
    ini_reader ini("./config.ini");
    bool ini_ok = ini.initialise();
    std::string section_name("section1");
    std::string key_str("key1");
    std::string return_str;
    bool got_config_ok = this_ini.get_config_name(section_name, key_str, &return_str);
    \todo Implement additional get_config_value functions with different return types and perform the type conversion in here.
    */
    class ini_reader
    {
        public:
            ini_reader(std::string);
            bool initialise(bool crash_on_fail = true );
            //bool get_config_value(std::string section_name, std::string key_name, std::string * return_config_value, bool verbose = true);
            //bool parse_names_config(std::string config_file_section, std::string config_key, std::vector<std::string> & results_vec, std::string delim=",");

        protected:
            std::string _config_file_path;
            const size_t _line_buffer_len = 255;
            bool _file_found = false;
            bool _file_valid = false;
            bool _initialised = false;
    };
}

#if 0
bool initialise()
{
    bool spiffs_ok = SPIFFS.begin();
    if(!spiffs_ok)
    {
        debug_print_loop_forever("ini_reader: could not start SPIFFS!");
    }
      
    File this_file = SPIFFS.open(_config_file_path.c_str());
    if(!this_file)
    {
        debug_print_loop_forever("ini_reader: failed to open the file!");
    }

    //Read the file
    while(this_file.available())
    {
        char line_buffer[255];
        int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
        buffer[l] = '\0';
        debug_print("Got line: ", true, false);
        debug_print(buffer);
        this.validate_ini_line(str(buffer));
        //Left and right strip
        //Check it:
        //- starts with [ and ends with ]
        //has a value to the left and an =
        //
    }
        std::string this_line = this_file.readline();
    }
    return;
    }

    bool validate_ini_line(const std::string this_line)
    {
        std::string clean_line = trim_copy(this_line);//remove whitespace
        bool is_section_head = false;
        if(clean_line.size() >= 3)
        {
            is_section_head = ((clean_line[0] == '[') && (clean_line[clean_line.size()-1] == ']'));
        }
        if(is_section_head)
        {
            return true;
        }

        bool is_section_head = ((clean_line))
    }

    //const size_t _line_buffer_len = 80;//Buffer is the max length of any one line (not the whole file)
  char buffer[_line_buffer_len];

  const char *filename = _config_file_path.c_str(); //"/test.ini";
  
  //Mount the SPIFFS  
  if (!SPIFFS.begin())
    while (1)
    {
      Serial.println("SPIFFS.begin() failed");

  SPIFFSIniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    // Cannot do anything else
    while (1)
      ;
  }else{
    _file_found = true;
     //Serial.println("Ini file exists");
  }
  
  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, _line_buffer_len)) {

    // Cannot do anything else
    while (1)
    {
      Serial.print("ini file ");
      Serial.print(ini.getFilename());
      Serial.print(" not valid: ");
      delay(1000);
    }
  }else{
    //Serial.println("Ini file is valid");
    _file_valid = true;
  }

  _initialised = true;
  return _initialised;
}
#endif
#include "beluga_ini_reader.h"
#include "beluga_debug.h"

#include <sstream>

#include "../submodules/beluga_utils/src/beluga_string.h"

namespace beluga_utils
{
  /*!
  \brief Creates a ini file 
  @param config_file_path is the path to the .ini file that SPIFFS will use e.g. ./config.ini
  */
  ini_reader::ini_reader(std::string config_file_path)
  {
      _config_file_path = config_file_path;
  }

  /*!
  \brief Initialises the ini_reader
  \details Use SPIFFS to open the file, validate that it complies with the .ini format.
  @param crash_on_fail set to True if you want it to error_loop_forever if something goes wrong on launching SPIFFS, set to False (default) and it will return False if something goes wrong.
  \return True if the file is found, and is valid .ini. False otherwise if debug_print_loop_forever is False, or hang of debug_print_loop_forever is True
  Usage:
  ini_reader ini("./config.ini");
  bool ini_ok = ini.initialise();

  @todo: Replace SPIFFS with LittleFS and have better failure handling e.g. file not found.
  */
  bool ini_reader::initialise(bool crash_on_fail = false)
  {
      
    //_line_buffer_len is the max length of any one line (not the whole file)
    char buffer[_line_buffer_len];
    const char *this_filename = _config_file_path.c_str(); 
    
    //Start SPIFFS
    bool spiffs_ok = SPIFFS.begin();
    if(!spiffs_ok)
    {
      if(crash_on_fail)
      {
        debug_print_loop_forever("beluga_ini_reader: SPIFFS.begin() failed");
      }else{
        return false;
      }
    }

    File this_file;
    this_file = SPIFFS.open(this_filename.c_str(), "r");
    while(file.available())
    {
      char terminator_char = '\n';
      int l = file.readBytesUntil(terminator_char, buffer, sizeof(buffer)); //Terminator character is not returned
      //TODO: Last line of file?
      
      bool is_section_heading = (buffer[0] == '[') && (buffer[l-1] == ']');
      if(is_section_heading)
      {
        std::string this_heading = std::string(buffer[1], l-2); //Copy a fixed number of chars. If there are \0 within the string, problems!
        debug_print("Read config heading: ", true, false);
        debug_print(this_heading);
      }else{
        std::string this_line = std::string(buffer, l); //Copy a fixed number of chars. If there are \0 within the string, problems!
        debug_print("Read config line: ", true, false);
        debug_print(this_line);
      }
    }
    
    _initialised = true;
    return _initialised;
  }

  /*!
  \brief Retrieves data a given section-key pair.
  @param section_name is the section in the .ini to use. In the .ini it is in [square braces]. In the beluga library it's
    usually the name of some device.
  @param key_name is the particular key we want data from
  @param return_config_value is how we return the value, via a pointer. If there is no section-key pair to be found, this will be nullptr.
  @param verbose if True, prints state data over Serial. If False, it doesn't.
  \return True if this section-key pair is found.
  \details This is the primary function used to retrieve data from the config file.
  Usage:
  ini_reader ini("./config.ini");
  bool ini_ok = ini.initialise();
  std::string section_name("section1");
  std::string key_str("key1");
  std::string return_str;
  bool got_config_ok = this_ini.get_config_name(section_name, key_str, &return_str);
  We could have returned either as 
  \return True if the file is found, and is valid .ini. False otherwise.
  */
  bool ini_reader::get_config_value(std::string section_name, std::string key_name, std::string * return_config_value, bool verbose)
  {
      char buffer[_line_buffer_len];
      const char *this_filename = _config_file_path.c_str(); //"/test.ini";

      SPIFFSIniFile ini(this_filename);

      if (!ini.open()) {
          while (1)
          {
              Serial.println("Ini open failed");
              delay(1000);
          }
      }
    
      //const char * c_section = section_name.c_str();
      //const char * c_key = key_name.c_str();
      bool val_found = ini.getValue(section_name.c_str(), key_name.c_str(), buffer, _line_buffer_len) ;
      if(val_found)
      {
          std::string config_val(buffer);
          *return_config_value = config_val;
          if(verbose)
          {
            std::stringstream ss;
            ss << "section " << section_name << " has entry " << key_name <<" with value " << config_val;
            Serial.println(ss.str().c_str());
          }
      }else{
        if(verbose)
        {
          std::stringstream ss;
          ss << "Config not found: section " << section_name << " key " << key_name;
          Serial.println(ss.str().c_str());
        }
      }
      return val_found;

  }


  /*
  config_key == "sensor_names", "actuator_names", etc
  */
  bool ini_reader::parse_names_config(std::string config_file_section, std::string config_key, std::vector<std::string> & results_vec, std::string delim)
  {
      bool config_ok = false;
      std::string name_list_str;
      config_ok = get_config_value(config_file_section, config_key, &name_list_str );
      if(! config_ok )
      {
          return false;
      }
      //name_list_str is now a comma-delimited list of names of things: apple,Pear,POTATO
      //convert to lower-case
      std::vector<std::string> raw_names = beluga_utils::split_string(name_list_str, delim);
      //Convert to lowercase and store
      for(auto iter = raw_names.begin(); iter != raw_names.end(); iter++)
      {
          std::string this_name =  string_to_lower(*iter);
          results_vec.push_back(this_name);   
      }
      return true;
  }


}
#include "beluga_ini_reader.h"
#include "beluga_debug.h"

#include <sstream>
#include <algorithm>

#include "beluga_string.h"


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
  \brief Add a new section to the data map
  \details Check if the section name is already present, and if not add a new section map
  @param this_section_name the name of the new section
  \return True if the new section name is not present already, False if it is already present.
  */
  bool ini_reader::add_new_section_name(std::string this_section_name)
  {
    //Check if this section name is already present

      if(std::find(_section_names.begin(), _section_names.end(), this_section_name) != _section_names.end()) {
          /* v contains x */
          return false;
      } else {
          /* v does not contain x */
          _section_names.push_back(this_section_name);
          std::map<std::string, std::string> this_map;
          _data[this_section_name] = this_map;
          return true;
      }
  }

  /*!
  \brief Return this when initialisation fails.
  */
  bool ini_reader::initialise_return_failure(std::string error_message, bool crash_on_fail)
  {
    if(crash_on_fail)
    {
      beluga_utils::debug_print_loop_forever(error_message);
      assert(false);//Should not reach this line
    }else{
      beluga_utils::debug_print(error_message);
    }
    return false;

  }
    /*!
    \brief Print the config dictionary to serial as a tree
    \details Print config heading, and indented key:value pairs. Do nothing if not initialised.debug_print
    \return Void.
    */
    void ini_reader::print_config_to_serial()
    {
      if(! _initialised)
      {
        return;
      }
      for(auto iter1 = _data.begin(); iter1 != _data.end(); iter1++)
      {
        Serial.println(iter1->first.c_str());
        for(auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++)
        {
          Serial.print("\t");
          Serial.print(iter2->first.c_str());
          Serial.print(" : ");
          Serial.println(iter2->second.c_str());
        }
      }
    }


  /*!
  \brief Initialises the ini_reader
  \details Use SPIFFS to open the file, validate that it complies with the .ini format.
  @param crash_on_fail set to True if you want it to error_loop_forever if something goes wrong on launching SPIFFS, set to False (default) and it will return False if something goes wrong.
  \return True if the file is found, and is valid .ini. False otherwise if debug_print_loop_forever is False, or hang of debug_print_loop_forever is True
  Usage:
  ini_reader ini("./config.ini");
  bool crash_on_fail = true;
  bool ini_ok = ini.initialise(crash_on_fail);
  @todo: Replace SPIFFS with LittleFS and have better failure handling e.g. file not found.
  */
  bool ini_reader::initialise(bool crash_on_fail )
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
    this_file = SPIFFS.open(_config_file_path.c_str(), "r");
    std::string this_section_name = "";

    while(this_file.available())
    {
      char terminator_char = '\n';
      char comment_char = ';';

      int l = this_file.readBytesUntil(terminator_char, buffer, sizeof(buffer)); //Terminator character is not returned
      if(l == 0)
      {
        //Just a newline (terminator isn't returned)
        continue;
      }
      //TODO: Last line of file?
      
      bool is_section_heading = (buffer[0] == '[') && (buffer[l-1] == ']');
      if(is_section_heading)
      {
        std::string this_heading = std::string(buffer, l); //std::string(buffer[1], l-2); //Copy a fixed number of chars. If there are \0 within the string, problems!
        //Remove [ and ]
        this_heading = this_heading.substr(1, this_heading.size()-2);
        
        debug_print("Read config heading: ", true, false);
        debug_print(this_heading);
        bool new_section_ok = add_new_section_name(this_heading);
        if(! new_section_ok)
        {
          std::stringstream ss;
          ss << "Error adding config section " << this_heading << ": duplicate section name.";
          return initialise_return_failure(ss.str(), crash_on_fail);
        }

        this_section_name = this_heading;
        continue; //Get the config

      }else{
        std::string this_line = std::string(buffer, l); //Copy a fixed number of chars. If there are \0 within the string, problems!
        if(this_line[0] == comment_char)
        {
          continue; //Ignore comment
        }
        debug_print("Read config line: ", true, false);
        debug_print(this_line);
        add_new_config_data(this_section_name, this_line);
      }
    }
    
    _initialised = true;
    return _initialised;

  }

  bool ini_reader::add_new_config_data(std::string this_section_name, std::string this_data_str)
  {
    std::string this_delimiter = "=";
    std::vector<std::string> split_data = beluga_utils::split_string(this_data_str, this_delimiter);
    _data[this_section_name][split_data[0]] = split_data[1];

    return true;
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
  bool crash_on_fail = true;
  bool ini_ok = ini.initialise(crash_on_fail);
  std::string section_name("section1");
  std::string key_str("key1");
  std::string return_str;
  bool got_config_ok = this_ini.get_config_name(section_name, key_str, &return_str);
  We could have returned either as 
  \return True if the file is found, and is valid .ini. False otherwise.
  */
  bool ini_reader::get_config_value(std::string section_name, std::string key_name, std::string * return_config_value, bool verbose)
  {
    try
    {
      std::string this_value = _data[section_name][key_name];
      if(verbose)
      {
        std::stringstream ss;
        ss << "ini_reader::get_config_value: section name " << section_name << " has value: " << this_value;
        debug_print(ss.str());
      }
      *return_config_value = this_value;
      return true;
    }catch()
    {
      //Section-key pair not present.
    }
    return false;
  }
    
  void ini_reader::clear()
  {
    _data.clear();
  }
  




}
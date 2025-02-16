#pragma once
#include <Arduino.h>
#include "beluga_constants.h"
namespace beluga_utils
{
    bool debug_print_enable = true;
    /*!
    \brief Set the enable flag for serial debug messages
    \author Bryan Clarke
    \date 16/2/2025
    \details Set the flag for debug print turned on/off
    Usage: beluga_utils::set_debug_print_enable(true);
    */
    bool set_debug_print_enable(bool enable)
    {
        beluga_utils::debug_print_enable = enable;
        return debug_print_enable;
    }
    /*!
    \brief Print to serial
    \author Bryan Clarke
    \date 27/10/2024
    \details Print to serial, with a few flags for blocking/newlines/etc
    \todo Re-implement with multiple print functions and debug log levels supported e.g.: https://stackoverflow.com/questions/37837904/multilevel-debug-with-printf-in-embedded-c
    Usage: beluga_utils::debug_print("This message will be printed if debug_print_enable is true");
    beluga_utils::debug_print("This message will always be printed", force_print = true);
    */
    void debug_print(std::string s, bool add_newline  = true, bool force_print = false)
    {
        if(! debug_print_enable)
        {
            return false;
        }
        if(add_newline)
        {
            Serial.println(s.c_str());
        }else{
            Serial.print(s.c_str());
        }
    }

    /*!
    \brief Print an error message, forever
    \author Bryan Clarke
    \date 27/10/2024
    \details Print to serial, endlessly, with a fixed period between messages.
    Use this when the program is terminating due to an unrecoverable fault.
    Note that if you have a watchdog timer enabled, this won't be forever.
    Usage: beluga_utils::debug_print_loop_forever("Something went very wrong!");
    */
    void debug_print_loop_forever(std::string s, int period_s = 1)
    {
        while(1)
        {
            debug_print(s, force_print = true);
            delay(period_s * beluga_utils::MS_TO_S); //delay is in ms so we convert s -> ms
        }
    }

}
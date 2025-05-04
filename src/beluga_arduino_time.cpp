#include "beluga_arduino_time.h"
#include <Arduino.h>
#include <climits> //Needed for ULLONG_MAX

namespace beluga_utils
{
    unsigned long calculate_time_dt_ms(unsigned long time_prev_ms, unsigned long time_now_ms)
    {
        unsigned long t2_ms = time_now_ms;
        if(t2_ms == 0)
        {
            t2_ms = millis();
        }
        unsigned long t1_ms = time_prev_ms;
        unsigned long dt_ms;
        bool overflowed = t1_ms > t2_ms;
        if(overflowed) 
        {
            //Wraparound for overflow.
            dt_ms = ULLONG_MAX - t2_ms + t1_ms;
        }else{
            dt_ms = t2_ms - t1_ms;
        }
        return dt_ms;
    }
}
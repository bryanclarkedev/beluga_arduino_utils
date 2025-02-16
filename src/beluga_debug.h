#pragma once
namespace beluga_utils
{
    extern bool debug_print_enable;
    bool set_debug_print_enable(bool enable);
    void debug_print(std::string s, bool add_newline  = true, bool force_print = false);
    void debug_print_loop_forever(std::string s, int period_s = 1 );
   
}
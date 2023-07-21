#pragma once
#include <iostream>
#include <string>

namespace nemesis
{
    bool calculate(std::string& equation,
                   std::string format,
                   std::string filename,
                   int linecount,
                   bool noDecimal = true);

    bool calculate(const std::string& equation, std::string& result, bool no_decimal = false);
}

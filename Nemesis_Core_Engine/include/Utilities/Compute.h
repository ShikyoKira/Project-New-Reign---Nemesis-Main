#pragma once

#include <string>
#include <filesystem>

namespace nemesis
{
    bool calculate(std::string& equation,
                   const std::string& format,
                   const std::filesystem::path& filepath,
                   int linenum,
                   bool noDecimal = true);

    bool calculate(const std::string& equation, std::string& result, bool no_decimal = false);
}

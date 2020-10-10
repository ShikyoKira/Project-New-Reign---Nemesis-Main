#pragma once

#include <string>

namespace nemesis
{
    namespace syntax
    {
        std::string ForEach();
        std::string ForEach(std::string condition);

        std::string ModCode();
        std::string ModCode(std::string condition);

        std::string EndModCodeSyntax();

        std::string Original();

        std::string Close();

        std::string If();
        std::string If(std::string condition);

        std::string ElseIf();
        std::string ElseIf(std::string condition);

        std::string EndIf();

        std::string EndSyntax();
    }
}

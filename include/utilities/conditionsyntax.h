#pragma once

#include <string>

#include "utilities/condtype.h"

namespace nemesis
{
    namespace syntax
    {
        std::string DeleteLine();
        std::string Spaces();

        std::string OpenComment();
        std::string CloseComment();

        std::string Aster();
        std::string CloseAster();
        std::string Aster(const std::string& condition);
        std::string_view AsterCondition(const std::string& line);
        std::string_view AsterCondition(const nemesis::Line& line);

        std::string ForEach();
        std::string ForEach(const std::string& condition);
        std::string_view ForEachCondition(const std::string& line);
        std::string_view ForEachCondition(const nemesis::Line& line);

        std::string ModCode();
        std::string ModCode(const std::string& condition);
        std::string_view ModCodeCondition(const std::string& line);
        std::string_view ModCodeCondition(const nemesis::Line& line);

        std::string EndModCodeSyntax();

        std::string Original();
        std::string LowerOriginal();

        std::string Close();

        std::string If();
        std::string If(const std::string& condition);
        std::string_view IfCondition(const std::string& line);
        std::string_view IfCondition(const nemesis::Line& line);

        std::string ElseIf();
        std::string ElseIf(const std::string& condition);
        std::string_view ElseIfCondition(const std::string& line);
        std::string_view ElseIfCondition(const nemesis::Line& line);

        std::string Else();

        std::string EndIf();

        std::string EndSyntax();
    }
}

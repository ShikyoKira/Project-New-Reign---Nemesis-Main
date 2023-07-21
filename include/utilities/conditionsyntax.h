#pragma once

#include <string>

#include "utilities/condtype.h"

namespace nemesis
{
    namespace syntax
    {
        std::string DeleteLine() noexcept;
        std::string Spaces() noexcept;

        std::string OpenComment() noexcept;
        std::string CloseComment() noexcept;

        std::string Comment(const std::string& content) noexcept;

        std::string Aster() noexcept;
        std::string CloseAster() noexcept;
        std::string Aster(const std::string& condition) noexcept;
        std::string_view AsterCondition(const std::string& line);
        std::string_view AsterCondition(const nemesis::Line& line);

        std::string ForEach() noexcept;
        std::string ForEach(const std::string& condition) noexcept;
        std::string_view ForEachCondition(const std::string& line);
        std::string_view ForEachCondition(const nemesis::Line& line);

        std::string ModCode() noexcept;
        std::string ModCode(const std::string& condition) noexcept;
        std::string_view ModCodeCondition(const std::string& line);
        std::string_view ModCodeCondition(const nemesis::Line& line);

        std::string EndModCodeSyntax() noexcept;

        std::string ModOriginal() noexcept;
        std::string LowerOriginal() noexcept;

        std::string Close() noexcept;
        std::string ModClose() noexcept;

        std::string If() noexcept;
        std::string If(const std::string& condition) noexcept;
        std::string_view IfCondition(const std::string& line);
        std::string_view IfCondition(const nemesis::Line& line);

        std::string ElseIf() noexcept;
        std::string ElseIf(const std::string& condition) noexcept;
        std::string_view ElseIfCondition(const std::string& line);
        std::string_view ElseIfCondition(const nemesis::Line& line);

        std::string Else() noexcept;

        std::string EndIf() noexcept;

        std::string EndSyntax() noexcept;
    }
}

#pragma once

#include <string_view>

namespace nemesis
{
    enum class CondType
    {
        NONE,
        FOREACH,
        IF,
        ELSEIF,
        ELSE,
        ENDIF,
        MOD_CODE,
        ORIGINAL,
        LOWER_ORIGINAL,
        CLOSE,
        ASTERISK,
        MULTI_CHOICE,
        ALL
    };

    nemesis::CondType ParseConditionType(std::string_view line);
}

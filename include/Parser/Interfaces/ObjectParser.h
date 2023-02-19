#pragma once

namespace nemesis
{
    struct ObjectParser
    {
        virtual void ParseLine(const nemesis::Line& line) = 0;
    };
}

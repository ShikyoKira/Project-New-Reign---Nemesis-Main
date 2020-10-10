#pragma once

#include <string>

using uint = unsigned int;

namespace nemesis
{
	struct Line : public std::string
    {
        Line() {}
        Line(const char* _line, uint _linenum = 0)
            : std::string(_line)
        {
            linenum = _linenum;
        }
        Line(std::string _line, uint _linenum = 0)
            : std::string(_line)
        {
            linenum = _linenum;
        }

        uint linenum;
    };

    struct Wline : public std::wstring
    {
        Wline() {}
        Wline(const wchar_t* _line, uint _linenum = 0)
            : std::wstring(_line)
        {
            linenum = _linenum;
        }
        Wline(std::wstring _line, uint _linenum = 0)
            : std::wstring(_line)
        {
            linenum = _linenum;
        }

        uint linenum;
    };
}

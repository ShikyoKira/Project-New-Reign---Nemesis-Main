#ifndef VERSION_H_
#define VERSION_H_

#include "utilities/readtextfile.h"

inline std::string GetNemesisVersion()
{
    FileReader BehaviorFormat(std::string("version"));

    if (BehaviorFormat.GetFile())
    {
        std::string line;

        while (BehaviorFormat.GetLines(line))
        {
            boost::to_lower(line);

            if (line.find("version") == 0) { return line.substr(7); }
            else if (line.find("v") == 0)
            {
                return line.substr(1);
            }
        }
    }

    ErrorMessage(1092, "version");
    return "";
}

#endif

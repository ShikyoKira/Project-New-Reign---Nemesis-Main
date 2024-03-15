#ifndef VERSION_H_
#define VERSION_H_

#include "debugmsg.h"
#include "utilities/algorithm.h"
#include "utilities/readtextfile.h"

inline std::string GetNemesisVersion()
{
	FileReader BehaviorFormat(std::string("version"));

	if (BehaviorFormat.TryGetFile())
	{
		std::string line;

		while (BehaviorFormat.TryGetLines(line))
		{
			nemesis::to_lower(line);

			if (line.find("version") == 0)
			{
				return line.substr(7);
			}
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

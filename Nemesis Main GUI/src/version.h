#ifndef VERSION_H_
#define VERSION_H_

#include <boost/regex.hpp>
#include "Global.h"

std::string GetNemesisVersion()
{
	std::string line;
	char charline[2000];
	std::shared_ptr<TextFile> BehaviorFormat = std::make_shared<TextFile>("version");

	if (BehaviorFormat->GetFile())
	{
		while (fgets(charline, 2000, BehaviorFormat->GetFile()))
		{
			line = charline;

			if (line.back() == '\n')
			{
				line.pop_back();
			}

			boost::to_lower(line);

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
	throw -1;
}

#endif

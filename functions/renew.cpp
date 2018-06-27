#include "renew.h"

#pragma warning(disable:4503)

using namespace std;

void ClearTempBehaviors()
{
	vecstr filelist;
	string tempbehavior = "temp_behaviors\\";
	read_directory(tempbehavior, filelist);

	for (unsigned int i = 0; i < filelist.size(); ++i)
	{
		if (!boost::iequals(filelist[i], "xml"))
		{
			if (remove((tempbehavior + filelist[i]).c_str()) != 0)
			{
				WarningMessage(1006);
			}
		}
	}
}

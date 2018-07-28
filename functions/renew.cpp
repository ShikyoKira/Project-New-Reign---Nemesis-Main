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
				if (boost::filesystem::is_directory(tempbehavior + filelist[i]))
				{
					if (!boost::filesystem::remove_all(tempbehavior + filelist[i]))
					{
						WarningMessage(1006);
					}
				}
				else
				{
					WarningMessage(1006);
				}
			}
		}
	}
}

void ClearTempXml()
{
	vecstr filelist;
	string folder = "temp_behaviors\\xml\\";

	if (isFileExist(folder) && boost::filesystem::is_directory(folder))
	{
		read_directory(folder, filelist);

		for (unsigned int i = 0; i < filelist.size(); ++i)
		{
			if (remove((folder + filelist[i]).c_str()) != 0)
			{
				if (boost::filesystem::is_directory(folder + filelist[i]))
				{
					if (!boost::filesystem::remove_all(folder + filelist[i]))
					{
						WarningMessage(1009, filelist[i]);
					}
				}
				else
				{
					WarningMessage(1009, filelist[i]);
				}
			}
		}
	}
}

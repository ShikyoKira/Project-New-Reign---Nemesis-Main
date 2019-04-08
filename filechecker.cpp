#include "filechecker.h"
#include <boost\algorithm\string.hpp>

using namespace std;

bool FileCheck(bool isUpdate)
{
	string file = "alternate animation";
	DebugLogging("Initializing file check...");

	if (!isFileExist(file))
	{
		ErrorMessage(1001, file);
	}

	file = "alternate animation\\alternate animation.script";

	if (!isFileExist(file))
	{
		ErrorMessage(1092, file);
	}

	file = "alternate animation\\alternate animation 2.script";

	if (!isFileExist(file))
	{
		ErrorMessage(1092, file);
	}

	file = "behavior templates";

	if (!isFileExist(file))
	{
		ErrorMessage(1001, file);
	}

	CreateFolder("mod");

	if (!isUpdate)
	{
		file = "animationdata_list";

		if (!isFileExist(file))
		{
			ErrorMessage(1092, file);
		}

		file = "temp_behaviors";
	}

	file = "hkxcmd.exe";

	if (!isFileExist(file))
	{
		ErrorMessage(1092, file);
	}

	file = "languages";

	if (!isFileExist(file))
	{
		ErrorMessage(1001, file);
	}

	file = "languages\\english.txt";

	if (!isFileExist(file))
	{
		ErrorMessage(1092, file);
	}

	DebugLogging("File Check complete");
	return true;
}

bool PCEACheck()
{
	DebugLogging("Initializing PCEA Check...");
	string file = skyrimDataPath->GetDataPath() + "Nemesis PCEA.esp";

	if (isFileExist(file))
	{
		file = "alternate animation\\nemesis pcea.script";

		if (!isFileExist(file))
		{
			ErrorMessage(1092, file);
		}

		file = skyrimDataPath->GetDataPath() + "scripts\\Nemesis_PCEA_MCM.pex";

		if (!isFileExist(file))
		{
			ErrorMessage(1092, file);
		}
	}

	DebugLogging("PCEA Check complete");
	return true;
}

void behaviorActivateMod(vecstr behaviorPriority)
{
	unordered_map<string, vecstr> behaviorActivator;	// modcode, behavior; existence of the behavior in any of these
	vecstr modlist;
	string directory = "mod\\";
	read_directory(directory, modlist);

	for (auto& modcode : modlist)
	{
		string newpath = directory + modcode;
		vecstr behaviorlist;

		if (boost::filesystem::is_directory(newpath))
		{
			read_directory(newpath, behaviorlist);

			for (auto& behavior : behaviorlist)
			{
				if (!boost::iequals(behavior, "info.ini") && !boost::iequals(behavior, "_1stperson"))
				{
					behaviorActivator[modcode].push_back(boost::to_lower_copy(behavior));
				}
				else if (boost::iequals(behavior, "_1stperson"))
				{
					vecstr fpbehaviorlist;
					read_directory(newpath + "\\" + behavior, fpbehaviorlist);

					for (auto& fpbehavior : fpbehaviorlist)
					{
						behaviorActivator[modcode].push_back("_1stperson\\" + boost::to_lower_copy(fpbehavior));
					}
				}
			}
		}
	}

	for (auto& modcode : behaviorPriority)
	{
		if (behaviorActivator.find(modcode) != behaviorActivator.end())
		{
			for (auto& file : behaviorActivator[modcode])
			{
				activatedBehavior[file] = true;
			}
		}
	}
}

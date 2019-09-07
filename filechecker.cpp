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
		file = "cache\\animationdata_list";

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
	string file = nemesisInfo->GetDataPath() + "Nemesis PCEA.esp";

	if (isFileExist(file))
	{
		file = "alternate animation\\nemesis pcea.script";

		if (!isFileExist(file))
		{
			ErrorMessage(1092, file);
		}

		file = nemesisInfo->GetDataPath() + "scripts\\Nemesis_PCEA_MCM.pex";

		if (!isFileExist(file))
		{
			ErrorMessage(1092, file);
		}
	}
	else
	{
		return false;
	}

	DebugLogging("PCEA Check complete");
	return true;
}

void behaviorActivateMod(vecstr behaviorPriority)
{
	unordered_map<string, vecstr> behaviorActivator;	// modcode, behavior; existence of the behavior in any of these
	string directory = "mod\\";
	vecstr modlist;
	unsigned int i = 1;

	for (vecstr::reverse_iterator itr = behaviorPriority.rbegin(); itr != behaviorPriority.rend(); ++itr)
	{
		string modcode = *itr;
		string newpath = directory + modcode;
		DebugLogging("Mod Checked " + to_string(i) + ": " + modcode);
		interMsg(TextBoxMessage(1013) + " " + to_string(i++) + ": " + modcode);

		if (!isFileExist(newpath) || !boost::filesystem::is_directory(newpath)) continue;

		if (modcode == "gender")
		{
			activatedBehavior["gender*"] = true;
			continue;
		}

		vecstr behaviorlist;
		read_directory(newpath, behaviorlist);

		for (auto& behavior : behaviorlist)
		{
			if (!boost::iequals(behavior, "info.ini") && !boost::iequals(behavior, "_1stperson"))
			{
				activatedBehavior[boost::to_lower_copy(behavior)] = true;
			}
			else if (boost::iequals(behavior, "_1stperson"))
			{
				vecstr fpbehaviorlist;
				read_directory(newpath + "\\" + behavior, fpbehaviorlist);

				for (auto& fpbehavior : fpbehaviorlist)
				{
					activatedBehavior["_1stperson\\" + boost::to_lower_copy(fpbehavior)] = true;
				}
			}
		}
	}

	if (behaviorPriority.size() > 0) interMsg("");
}

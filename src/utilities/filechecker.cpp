#include "debuglog.h"

using namespace std;

void processFileError(pair<string_view, int> err);

void processFileError(pair<string_view, int> err)
{
	if (!isFileExist(err.first))
	{
		ErrorMessage(err.second, err.first);
	}
}

bool FileCheck(bool isUpdate)
{
	DebugLogging("Initializing file check...");
	using fileError = pair<string_view, int>;

	constexpr std::array<fileError, 7> filesToCheck =
	{
		fileError{"alternate animation", 1001},
		{"alternate animation\\alternate animation.script", 1092},
		{"alternate animation\\alternate animation 2.script", 1092},
		{"behavior templates", 1001},
		{"hkxcmd.exe", 1092},
		{"languages", 1001},
		{"languages\\english.txt", 1092}
	};

	constexpr std::array<fileError, 1> filesToNotCheckInUpdate =
	{
		fileError{"cache\\animationdata_list", 1092}
	};

	for (const auto& pair : filesToCheck)
	{
		processFileError(pair);
	}

	CreateFolder("mod");

	if (!isUpdate)
	{
		for (const auto& pair : filesToNotCheckInUpdate)
		{
			processFileError(pair);
		}
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

		if (!isFileExist(file)) ErrorMessage(1092, file);

		file = nemesisInfo->GetDataPath() + "scripts\\Nemesis_PCEA_MCM.pex";

		if (!isFileExist(file)) ErrorMessage(1092, file);
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

		if (!isFileExist(newpath) || !std::filesystem::is_directory(newpath)) continue;

		if (modcode == "gender")
		{
			activatedBehavior["gender*"] = true;
			continue;
		}

		vecstr behaviorlist;
		read_directory(newpath, behaviorlist);

		for (auto& behavior : behaviorlist)
		{
			if (!nemesis::iequals(behavior, "info.ini") && !nemesis::iequals(behavior, "_1stperson"))
			{
				activatedBehavior[nemesis::to_lower_copy(behavior)] = true;
			}
			else if (nemesis::iequals(behavior, "_1stperson"))
			{
				vecstr fpbehaviorlist;
				read_directory(newpath + "\\" + behavior, fpbehaviorlist);

				for (auto& fpbehavior : fpbehaviorlist)
				{
					activatedBehavior["_1stperson\\" + nemesis::to_lower_copy(fpbehavior)] = true;
				}
			}
		}
	}

	if (behaviorPriority.size() > 0) interMsg("");
}

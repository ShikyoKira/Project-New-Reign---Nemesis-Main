#include "filechecker.h"

using namespace std;

bool FileCheck()
{
	string file = "alternate animation";

	if (!isFileExist(file))
	{
		cout << "ERROR(1096): " << file << "folder not found. Please reinstall Nemesis" << endl << endl;
		error = true;
		return false;
	}

	file = file + "\\alternate animation.script";

	if (!isFileExist(file))
	{
		cout << "ERROR(1096): " << file << "folder not found. Please reinstall Nemesis" << endl << endl;
		error = true;
		return false;
	}

	file = "behavior templates";

	if (!isFileExist(file))
	{
		cout << "ERROR(1096): " << file << "folder not found. Please reinstall Nemesis" << endl << endl;
		error = true;
		return false;
	}

	file = "mod";

	if (!isFileExist(file))
	{
		CreateDirectory(file.c_str(), NULL);
	}

	file = "animationdata_list.txt";

	if (!isFileExist(file))
	{
		cout << "ERROR(1096): " << file << "folder not found. Please reinstall Nemesis" << endl << endl;
		error = true;
		return false;
	}

	file = "hkxcmd.exe";

	if (!isFileExist(file))
	{
		cout << "ERROR(1096): " << file << "folder not found. Please reinstall Nemesis" << endl << endl;
		error = true;
		return false;
	}

	return true;
}

void behaviorActivateMod(vecstr behaviorPriority)
{
	unordered_map<string, vecstr> behaviorActivator;	// modcode, behavior; existence of the behavior in any of these
	vecstr modlist;
	string directory = "mod\\";
	read_directory(directory, modlist);

	for (unsigned int i = 0; i < modlist.size(); ++i)
	{
		string newpath = directory + modlist[i];
		vecstr behaviorlist;

		if (boost::filesystem::is_directory(newpath))
		{
			read_directory(newpath, behaviorlist);

			for (unsigned int j = 0; j < behaviorlist.size(); ++j)
			{
				behaviorActivator[modlist[i]].push_back(behaviorlist[j]);
			}
		}
	}

	for (unsigned int i = 0; i < behaviorPriority.size(); ++i)
	{
		if (behaviorActivator.find(behaviorPriority[i]) != behaviorActivator.end())
		{
			for (unsigned int j = 0; j < behaviorActivator[behaviorPriority[i]].size(); ++j)
			{
				activatedBehavior[behaviorActivator[behaviorPriority[i]][j]] = true;
			}
		}
	}
}

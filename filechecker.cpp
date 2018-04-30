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

	file = "cache";

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
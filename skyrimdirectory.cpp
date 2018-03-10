#include "skyrimdirectory.h"

using namespace std;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;

	bf::path path(bf::current_path());
	string currentPath = path.string();

	if (currentPath.find("\\Data\\", 0) != string::npos || currentPath.find("\\data\\", 0) != string::npos)
	{
		bf::path tempPath = path;

		while (true)
		{
			if ((tempPath.parent_path().string()).find("\\Data", 0) == string::npos && (tempPath.parent_path().string()).find("\\data", 0) == string::npos)
			{
				break;
			}

			tempPath = tempPath.parent_path();
		}

		dataPath = tempPath.string() + "\\Data\\";
	}
	else
	{
		cout << "ERROR(1009): Unable to locate Skyrim Data found. This tool must be executed in skyrim's data file." << endl;
		error = true;
		return;
	}
}

string DataPath::GetDataPath()
{
	return dataPath;
}
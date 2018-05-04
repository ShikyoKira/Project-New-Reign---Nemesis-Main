#include "skyrimdirectory.h"

using namespace std;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;

	bf::path path(bf::current_path());
	string currentPath = path.string();

	if (currentPath.find("\\Data\\", 0) != NOT_FOUND || currentPath.find("\\data\\", 0) != NOT_FOUND)
	{
		bf::path tempPath = path;

		while (true)
		{
			if ((tempPath.parent_path().string()).find("\\Data", 0) == NOT_FOUND && (tempPath.parent_path().string()).find("\\data", 0) == NOT_FOUND)
			{
				break;
			}

			tempPath = tempPath.parent_path();
		}

		dataPath = tempPath.string() + "\\Data\\";
	}
	else
	{
		ErrorMessage(1008);
		return;
	}
}

string DataPath::GetDataPath()
{
	return dataPath;
}
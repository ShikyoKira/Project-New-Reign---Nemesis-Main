#include "skyrimdirectory.h"

using namespace std;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;

	bf::path path(bf::current_path());
	string currentPath = path.string();

	if (wordFind(currentPath, "\\Data\\") != NOT_FOUND)
	{
		bf::path tempPath = path;

		while (true)
		{
			if (wordFind(tempPath.parent_path().string(), "\\Data") == NOT_FOUND)
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
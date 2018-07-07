#include "skyrimdirectory.h"

using namespace std;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;
	bf::path path;

	if (bf::current_path().string() == "E:\\C++\\Project 2\\Nemesis Main GUI")
	{
		path = bf::path("E:\\C++\\Project 2\\Nemesis Main GUI\\data\\meshes");
	}
	else
	{
		path = bf::path(bf::current_path());
	}

	if (wordFind(path.string(), "\\Data\\") != NOT_FOUND)
	{
		while (true)
		{
			if (wordFind(path.parent_path().string(), "\\Data") == NOT_FOUND)
			{
				break;
			}

			path = path.parent_path();
		}

		dataPath = path.string();
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
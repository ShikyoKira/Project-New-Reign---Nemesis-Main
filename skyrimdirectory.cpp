#include "skyrimdirectory.h"
#include <Windows.h>

using namespace std;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;

	if (bf::current_path().string() == "E:\\C++\\Project 2\\Nemesis Main GUI")
	{
		dataPath = "E:\\C++\\Project 2\\Nemesis Main GUI\\data\\";
	}
	else
	{
		if (wordFind(bf::current_path().string(), "\\Data\\") != NOT_FOUND)
		{
			DWORD dwType = REG_SZ;
			HKEY hKey = 0;
			char value[1024];
			DWORD value_length = 1024;
			const char* subkey = "SOFTWARE\\Bethesda Softworks\\Skyrim";
			RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hKey);
			RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
			dataPath = value;
			dataPath = dataPath + "Dataasd";

			if (!isFileExist(dataPath))
			{
				const char* subkey = "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim";
				RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hKey);
				RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
				dataPath = value;
				dataPath = dataPath + "Data\\";

				if (!isFileExist(dataPath))
				{
					ErrorMessage(6005);
					return;
				}
			}
		}
		else
		{
			ErrorMessage(1008);
			return;
		}
	}

}

string DataPath::GetDataPath()
{
	return dataPath;
}
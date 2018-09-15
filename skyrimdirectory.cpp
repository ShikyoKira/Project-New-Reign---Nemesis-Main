#include "skyrimdirectory.h"
#include <Windows.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

bool SSE = false;

DataPath::DataPath()
{
	namespace bf = boost::filesystem;

	if (bf::current_path().string() == "E:\\C++\\Project 2\\Nemesis Main GUI")
	{
		dataPath = "E:\\C++\\Project 2\\Nemesis Main GUI\\data\\";
	}
	else
	{
		size_t pos = wordFind(bf::current_path().string(), "\\Data\\");

		if (pos != NOT_FOUND)
		{
			vecstr filelist;
			read_directory(bf::current_path().string().substr(0, pos), filelist);

			for (auto& file : filelist)
			{
				if (boost::iequals(file, "SkyrimSE.exe"))
				{
					SSE = true;
					break;
				}
				else if (boost::iequals(file, "binkw64.dll"))
				{
					SSE = true;
					break;
				}
				else if (boost::iequals(file, "binkw32.dll"))
				{
					break;
				}
			}

			DWORD dwType = REG_SZ;
			HKEY hKey = 0;
			char value[1024];
			DWORD value_length = 1024;

			if (SSE)
			{
				RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
			}
			else
			{
				RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Bethesda Softworks\\Skyrim", &hKey);
			}

			RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
			dataPath = value;
			dataPath = dataPath + "Data\\";

			if (!isFileExist(dataPath))
			{
				if (SSE)
				{
					RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
				}
				else
				{
					RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim", &hKey);
				}

				RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
				dataPath = value;
				dataPath = dataPath + "Data\\";

				if (!isFileExist(dataPath))
				{
					bf::path pirated = bf::current_path();

					size_t counter = count(pirated.string().begin(), pirated.string().end(), '\\');
					size_t i = 0;

					while (i < counter)
					{
						string temp = pirated.string();
						temp = boost::regex_replace(string(boost::to_lower_copy(temp)), boost::regex(".*\\d(at)a$"), string("\\1"));

						if (temp != pirated.string())
						{
							dataPath = pirated.string();
							break;
						}

						pirated = pirated.parent_path();
						++i;
					}

					if (dataPath.back() == '\\')
					{
						ErrorMessage(6005);
						interMsg(dataPath);
						return;
					}
					else
					{
						dataPath += "\\";
					}
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
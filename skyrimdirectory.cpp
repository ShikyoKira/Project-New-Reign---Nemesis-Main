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
		string curpath;

		{
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			curpath = string(buffer).substr(0, string(buffer).find_last_of("\\/"));
		}

		size_t pos = wordFind(curpath, "\\Data\\");

		if (pos != NOT_FOUND)
		{
			vecstr filelist;
			string skyrimDataDirect;

			{
				bf::path path(curpath);
				size_t counter = count(curpath.begin(), curpath.end(), '\\');
				size_t i = 0;

				while (i < counter)
				{
					if (boost::iequals(path.stem().string(), "data"))
					{
						skyrimDataDirect = path.string();
						break;
					}

					path = path.parent_path();
					++i;
				}

				read_directory(path.parent_path().string(), filelist);
			}

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

			// get skyrim data directory from registry key
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

			LONG result = RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);

			dataPath = value;
			dataPath = dataPath + "Data\\";

			if (result != ERROR_SUCCESS || !isFileExist(dataPath))
			{
				if (SSE)
				{
					RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
				}
				else
				{
					RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim", &hKey);
				}

				result = RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
				dataPath = value;
				dataPath = dataPath + "Data\\";

				// data path directly from address
				if (result != ERROR_SUCCESS || !isFileExist(dataPath))
				{
					dataPath = skyrimDataDirect;

					if (dataPath.back() == '\\')
					{
						ErrorMessage(6005);
						interMsg(dataPath);
						return;
					}
					else
					{
						dataPath.append("\\");
					}
				}
			}
		}
		else
		{
			ErrorMessage(1008);
			interMsg("Detected Path: " + curpath);
			DebugLogging("Detected Path: " + curpath);
			return;
		}
	}
}

string DataPath::GetDataPath()
{
	return dataPath;
}
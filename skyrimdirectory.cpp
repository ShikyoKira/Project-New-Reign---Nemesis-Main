#include "skyrimdirectory.h"
#include "Nemesis Main GUI\src\utilities\wstrconvert.h"
#include <Windows.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <QtCore/QCoreApplication.h>

using namespace std;

bool SSE = false;
unsigned int maxAnim = 10000;

DataPath::DataPath()
{
	if (isFileExist("nemesis.ini"))
	{
		vecstr storeline;

		if (GetFunctionLines("nemesis.ini", storeline))
		{
			for (auto& line : storeline)
			{
				string path = boost::regex_replace(string(line), boost::regex(".*[\\s]*=[\\s]*(.*)"), string("\\1"));

				if (!boost::iequals(path, "auto"))
				{
					string input = boost::regex_replace(string(line), boost::regex("(.*)[\\s]*=[\\s]*.*"), string("\\1"));

					if (boost::iequals(input, "skyrimdatadirectory"))
					{
						dataPath = path;
					}
					else if (boost::iequals(input, "maxanimation") && isOnlyNumber(path))
					{
						maxAnim = stoi(path);
					}
				}
			}
		}
	}	

	namespace bf = boost::filesystem;
	string curpath = wstrConv.to_bytes(QCoreApplication::applicationDirPath().toStdWString());
	replace(curpath.begin(), curpath.end(), '/', '\\');

	if (dataPath.length() == 0)
	{
		if (bf::current_path().string() == "E:\\C++\\Project 2\\Nemesis Main GUI")
		{
			dataPath = "E:\\C++\\Project 2\\Nemesis Main GUI\\data\\";
			curpath = dataPath + "nemesis_engine";
		}
		else
		{
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

				if (SSE) RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
				else RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Bethesda Softworks\\Skyrim", &hKey);

				LONG result = RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);

				dataPath = value;
				dataPath = dataPath + "Data\\";

				if (result != ERROR_SUCCESS || !isFileExist(dataPath))
				{
					if (SSE) RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
					else RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim", &hKey);

					result = RegQueryValueEx(hKey, "installed path", NULL, &dwType, (LPBYTE)&value, &value_length);
					dataPath = value;
					dataPath = dataPath + "Data\\";

					// data path directly from address
					if (result != ERROR_SUCCESS || !isFileExist(dataPath))
					{
						dataPath = skyrimDataDirect;

						if (dataPath.back() == '\\')
						{
							interMsg(dataPath);
							ErrorMessage(6005);
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
				interMsg("Detected Path: " + curpath);
				DebugLogging("Detected Path: " + curpath);
				ErrorMessage(1008);
			}
		}
	}

	if (boost::to_lower_copy(dataPath + "nemesis_engine") != boost::to_lower_copy(curpath)) ErrorMessage(6010, curpath, dataPath + "nemesis_engine");
}

string DataPath::GetDataPath()
{
	return dataPath;
}
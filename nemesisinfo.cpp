#include <Windows.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <QtCore/QFile.h>
#include <QtCore/QTextStream.h>
#include <QtCore/QCoreApplication.h>

#include "nemesisinfo.h"

#include "functions\writetextfile.h"

#include "Nemesis Main GUI\src\utilities\wstrconvert.h"

using namespace std;

bool SSE = false;

void NemesisInfo::iniFileUpdate()
{
	QFile file("nemesis.ini");

	if (file.open(QIODevice::Truncate | QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << QString::fromStdString("SkyrimDataDirectory=" + dataPath + "\r\n");
		stream << QString::fromStdString("MaxAnimation=" + to_string(maxAnim) + "\r\n");
		stream << QString::fromStdString("first=" + string(first ? "true" : "false") + "\r\n");
		stream << QString::fromStdString("width=" + to_string(width) + "\r\n");
		stream << QString::fromStdString("height=" + to_string(height) + "\r\n");
		stream << QString::fromStdString("modNameWidth=" + to_string(modNameWidth) + "\r\n");
		stream << QString::fromStdString("authorWidth=" + to_string(authorWidth) + "\r\n");
		stream << QString::fromStdString("priorityWidth=" + to_string(priorityWidth) + "\r\n");
		stream.flush();
		file.close();
	}
}

NemesisInfo::NemesisInfo()
{
	try
	{
		setup();
	}
	catch (nemesis::exception&) {}
}

NemesisInfo::NemesisInfo(bool& exception)
{
	try
	{
		setup();
	}
	catch (nemesis::exception&)
	{
		exception = true;
	}
}

void NemesisInfo::setup()
{
	set<string> hasAuto;
	bool force = false;

	if (isFileExist("nemesis.ini"))
	{
		vecstr storeline;

		if (GetFunctionLines(string("nemesis.ini"), storeline))
		{
			for (auto& line : storeline)
			{
				string path = boost::regex_replace(string(line), boost::regex(".*[\\s]*=[\\s]*(.*)"), string("\\1"));
				string input = boost::to_lower_copy(boost::regex_replace(string(line), boost::regex("(.*)[\\s]*=[\\s]*.*"), string("\\1")));

				if (!boost::iequals(path, "auto"))
				{
					if (boost::iequals(input, "skyrimdatadirectory"))
					{
						if (isFileExist(path) && wordFind(path, "data") != NOT_FOUND)
						{
							dataPath = path;
							force = true;

							if (dataPath.back() != '\\' && dataPath.back() != '/')
							{
								if (dataPath.find("\\") != NOT_FOUND) dataPath.push_back('\\');
								else dataPath.push_back('/');
							}

							vecstr filelist;
							boost::filesystem::path fspath(dataPath);

							while (!boost::iequals(fspath.stem().string(), "data"))
							{
								fspath = fspath.parent_path();
							}

							read_directory(fspath.parent_path().string(), filelist);

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
						}
					}
					else if (boost::iequals(input, "maxanimation") && isOnlyNumber(path)) maxAnim = stoi(path);
					else if (boost::iequals(input, "first")) first = path != "false";
					else if (boost::iequals(input, "height")) height = stoi(path);
					else if (boost::iequals(input, "width")) width = stoi(path);
					else if (boost::iequals(input, "modNameWidth")) modNameWidth = stoi(path);
					else if (boost::iequals(input, "authorWidth")) authorWidth = stoi(path);
					else if (boost::iequals(input, "priorityWidth")) priorityWidth = stoi(path);
				}
				else if (input == "skyrimdatadirectory" || input == "maxanimation" || input == "first")
				{
					hasAuto.insert(input);
				}
			}
		}
	}

	namespace bf = boost::filesystem;
	string curpath = wstrConv.to_bytes(QCoreApplication::applicationDirPath().toStdWString());
	std::replace(curpath.begin(), curpath.end(), '/', '\\');

	if (bf::current_path().string() == "E:\\C++\\Project 2\\Nemesis Main GUI")
	{
		dataPath = "E:\\C++\\Project 2\\Nemesis Main GUI\\data\\";
		curpath = dataPath + "nemesis_engine";
	}

	if (dataPath.length() == 0)
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

	if (!force && boost::to_lower_copy(dataPath + "nemesis_engine") != boost::to_lower_copy(curpath)) ErrorMessage(6010, curpath, dataPath + "nemesis_engine");

	iniFileUpdate();
}

string NemesisInfo::GetDataPath()
{
	return dataPath;
}

unsigned int NemesisInfo::GetMaxAnim()
{
	return maxAnim;
}

unsigned int NemesisInfo::GetAuthorWidth()
{
	return authorWidth;
}

unsigned int NemesisInfo::GetPriorityWidth()
{
	return priorityWidth;
}

void NemesisInfo::setFirst(bool _first)
{
	first = _first;
}

void NemesisInfo::setWidth(unsigned int _width)
{
	width = _width;
}

void NemesisInfo::setHeight(unsigned int _height)
{
	height = _height;
}

void NemesisInfo::setModNameWidth(unsigned int _width)
{
	modNameWidth = _width;
}

void NemesisInfo::setAuthorWidth(unsigned int _width)
{
	authorWidth = _width;
}

void NemesisInfo::setPriorityWidth(unsigned int _width)
{
	priorityWidth = _width;
}

bool NemesisInfo::IsFirst()
{
	return first;
}

unsigned int NemesisInfo::GetWidth()
{
	return width;
}

unsigned int NemesisInfo::GetHeight()
{
	return height;
}

unsigned int NemesisInfo::GetModNameWidth()
{
	return modNameWidth;
}

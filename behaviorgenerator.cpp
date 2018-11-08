#include "behaviorgenerator.h"
#include "add animation/alternateanimation.h"
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/algorithm/string.hpp>
#include <Nemesis Main GUI\lua-5.3.5\src\lua.hpp>

#pragma warning(disable:4503)

using namespace std;

bool hkxcmdProcess(string xmlfile, string hkxfile)
{

	string args;

	if (SSE)
	{
		args = "convert -v:AMD64 \"" + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";
	}
	else
	{
		args = "convert -v:WIN32 \"" + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";
	}

	if (boost::process::system("hkxcmd " + args, boost::process::windows::hide) != 0)
	{
		ErrorMessage(1003, xmlfile);
		return false;
	}
	else
	{
		if (!isFileExist(hkxfile + ".hkx"))
		{
			ErrorMessage(1003, xmlfile);
			return false;
		}
	}

	return true;
}

bool hkxcmdXmlInput(string hkxfile, vecstr& fileline)
{
	string xmlfile = hkxfile + ".xml";
	string args = "convert -v:xml \"" + hkxfile + ".hkx\" \"" + xmlfile + "\"";

	if (boost::process::system("hkxcmd " + args, boost::process::windows::hide) != 0)
	{
		ErrorMessage(1003, hkxfile);
		return false;
	}
	else
	{
		if (!isFileExist(xmlfile))
		{
			ErrorMessage(1003, hkxfile);
			return false;
		}
		else if (!boost::filesystem::is_directory(xmlfile))
		{
			if (!GetFunctionLines(xmlfile, fileline))
			{
				return false;
			}

			if (fileline.size() == 0)
			{
				ErrorMessage(3001, xmlfile);
				return false;
			}

			if (!boost::filesystem::remove(xmlfile))
			{
				ErrorMessage(1082, xmlfile, xmlfile);
				return false;
			}
		}
	}

	return true;
}

void RunScript(string directory)
{
	if (!isFileExist(directory))
	{
		FolderCreate(directory);
		return;
	}

	bool warning = false;
	vecstr scriptlist;
	read_directory(directory, scriptlist);

	for (auto& file : scriptlist)
	{
		string scriptpath = directory + file;
		boost::filesystem::path scriptfile(scriptpath);

		// hidden scripts
		if (!boost::filesystem::is_directory(scriptfile))
		{
			if (boost::iequals(scriptfile.extension().string(), ".bat"))
			{
				try
				{
					if (boost::process::system(scriptpath, boost::process::windows::hide) != 0)
					{
						WarningMessage(1023, scriptpath);
						warning = true;
					}
				}
				catch (const std::exception& ex)
				{
					ErrorMessage(6008, ex.what());
					return;
				}
			}
			else if (boost::iequals(scriptfile.extension().string(), ".lua"))
			{
				lua_State* luascript = luaL_newstate();
				luaL_openlibs(luascript);
				luaL_dofile(luascript, scriptpath.c_str());
				lua_close(luascript);
			}
		}
		// visible scripts
		else if (boost::iequals(file, "show") && boost::filesystem::is_directory(scriptfile))
		{
			vecstr shownscriptlist;
			read_directory(scriptpath, shownscriptlist);

			for (auto& shown : shownscriptlist)
			{
				string shownpath = scriptpath + "\\" + shown;
				boost::filesystem::path shownscript(shownpath);

				if (!boost::filesystem::is_directory(shownscript))
				{
					if (boost::iequals(shownscript.extension().string(), ".bat"))
					{
						try
						{
							if (boost::process::system(shownpath, boost::process::windows::hide) != 0)
							{
								WarningMessage(1023, shownscript);
								warning = true;
							}
						}
						catch (const std::exception& ex)
						{
							ErrorMessage(6008, ex.what());
							return;
						}
					}
					else if (boost::iequals(shownscript.extension().string(), ".lua"))
					{
						lua_State* luascript = luaL_newstate();
						luaL_openlibs(luascript);
						luaL_dofile(luascript, scriptpath.c_str());
						lua_close(luascript);
					}
				}
			}
		}
	}

	if (warning)
	{
		interMsg("");
	}
}

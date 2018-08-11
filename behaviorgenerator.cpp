#include "behaviorgenerator.h"
#include "add animation/alternateanimation.h"
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/algorithm/string.hpp>

#pragma warning(disable:4503)

using namespace std;

bool hkxcmdProcess(string xmlfile, string hkxfile)
{
	string args = "convert -v:WIN32 \"" + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";

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
		else
		{
			GetFunctionLines(xmlfile, fileline);

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

	for (auto& scriptfile : scriptlist)
	{
		string scriptpath = directory + scriptfile;
		boost::filesystem::path script(scriptpath);

		if (!boost::filesystem::is_directory(script) && boost::iequals(script.extension().string(), ".bat"))
		{
			if (boost::process::system(scriptpath, boost::process::windows::hide) != 0)
			{
				WarningMessage(1023, scriptpath);
				warning = true;
			}
		}
		else if (boost::iequals(scriptfile, "show") && boost::filesystem::is_directory(script))
		{
			vecstr hiddenscriptlist;
			read_directory(scriptpath, hiddenscriptlist);

			for (auto& hidden : hiddenscriptlist)
			{
				string hiddenpath = scriptpath + "\\" + hidden;
				boost::filesystem::path hiddenscript(hiddenpath);

				if (!boost::filesystem::is_directory(hiddenscript) && boost::iequals(hiddenscript.extension().string(), ".bat"))
				{
					if (boost::process::system(hiddenpath) != 0)
					{
						WarningMessage(1023, hiddenpath);
						warning = true;
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

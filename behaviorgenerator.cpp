#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/algorithm/string.hpp>
#include "behaviorgenerator.h"
#include "add animation/alternateanimation.h"
#include "Nemesis Main GUI\src\utilities\filerelease.h"

#pragma warning(disable:4503)

using namespace std;

bool hkxcmdProcess(string xmlfile, string hkxfile)
{
	string args = string(SSE ? "convert -v:AMD64 \"" : "convert -v:WIN32 \"") + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";

	if (error) throw nemesis::exception();

	if (boost::process::system("hkxcmd " + args, boost::process::windows::hide) != 0) ErrorMessage(1003, xmlfile);
	else if (!isFileExist(hkxfile + ".hkx")) ErrorMessage(1003, xmlfile);

	return true;
}

bool hkxcmdXmlInput(string hkxfile, vecstr& fileline)
{
	string xmlfile = hkxfile + ".xml";
	string args = "convert -v:xml \"" + hkxfile + ".hkx\" \"" + xmlfile + "\"";

	if (boost::process::system("hkxcmd " + args, boost::process::windows::hide) != 0)
	{
		ErrorMessage(1207, hkxfile);
	}
	else if (!isFileExist(xmlfile))
	{
		ErrorMessage(1207, hkxfile);
	}
	else if (!boost::filesystem::is_directory(xmlfile))
	{
		if (!GetFunctionLines(xmlfile, fileline)) return false;

		if (fileline.size() == 0) ErrorMessage(3001, xmlfile);

		if (ReleaseLockedFile(xmlfile) && !boost::filesystem::remove(xmlfile)) ErrorMessage(1082, xmlfile, xmlfile);
	}

	return true;
}

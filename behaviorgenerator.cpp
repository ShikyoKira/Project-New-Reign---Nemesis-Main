#include "behaviorgenerator.h"
#include <boost/process.hpp>
#include <boost/process/windows.hpp>

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

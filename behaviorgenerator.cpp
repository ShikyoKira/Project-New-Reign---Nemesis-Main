#include <boost\atomic.hpp>

#include <QtCore\QProcess>

#include "behaviorgenerator.h"

#include "add animation\alternateanimation.h"

#pragma warning(disable:4503)

using namespace std;

boost::atomic_flag failedBehaviorFlag = BOOST_ATOMIC_FLAG_INIT;
vecstr failedBehaviors;

bool hkxcmdProcess(string xmlfile, string hkxfile, bool last)
{
	string args = string(SSE ? "convert -v:AMD64 \"" : "convert -v:WIN32 \"") + xmlfile + ".xml\" \"" + hkxfile + ".hkx\"";

	if (error) throw nemesis::exception();

	if (QProcess::execute("hkxcmd " + QString::fromStdString(args)) != 0 || !isFileExist(hkxfile + ".hkx"))
	{
		if (last) ErrorMessage(1003, xmlfile);
		
		while (failedBehaviorFlag.test_and_set(boost::memory_order_acquire));
		failedBehaviors.push_back(xmlfile);
		failedBehaviors.push_back(hkxfile); 
		failedBehaviorFlag.clear(boost::memory_order_release);

		return false;
	}

	return true;
}

bool hkxcmdXmlInput(string hkxfile, vecstr& fileline)
{
	string xmlfile = hkxfile + ".xml";
	string args = "convert -v:xml \"" + hkxfile + ".hkx\" \"" + xmlfile + "\"";

	if (QProcess::execute("hkxcmd " + QString::fromStdString(args)) != 0 || !isFileExist(xmlfile))
	{
		ErrorMessage(1207, hkxfile);
	}
	else if (!boost::filesystem::is_directory(xmlfile))
	{
		if (!GetFunctionLines(xmlfile, fileline)) return false;

		if (fileline.size() == 0) ErrorMessage(3001, xmlfile);

		if (!boost::filesystem::remove(xmlfile)) ErrorMessage(1082, xmlfile, xmlfile);
	}

	return true;
}

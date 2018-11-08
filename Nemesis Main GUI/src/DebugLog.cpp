#include "DebugLog.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

vecstr updatelog;
vecstr patchlog;
boost::mutex debugmutex;
string filename = "CriticalLog.txt";

string currentTime();

void DebugOutput()
{
	filename.clear();
}

string currentTime()
{
	return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
}

void DebugLogging(string line, bool noEndLine)
{
	int size = count(line.begin(), line.end(), '\n');

	if (noEndLine)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			line.replace(line.find("\n"), 1, " | ");
		}
	}

	boost::lock_guard<boost::mutex> locker(debugmutex);
	ofstream relog(filename, ios_base::app);
	relog << "[" + currentTime() + "] " + line + "\n";
	relog.close();
}

void UpdateReset()
{
	filename = "UpdateLog.txt";
	ofstream log(filename);
	log.close();
}

void PatchReset()
{
	filename = "PatchLog.txt";
	ofstream log(filename);
	log.close();
}

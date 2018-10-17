#include "DebugLog.h"
#include <atomic>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

vecstr updatelog;
vecstr patchlog;
atomic_flag debuglock = ATOMIC_FLAG_INIT;
string filename;

string currentTime();

void DebugOutput()
{
	filename.clear();
}

string currentTime()
{
	return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
}

void DebugLogging(string line)
{
	int size = count(line.begin(), line.end(), '\n');

	for (unsigned int i = 0; i < size; ++i)
	{
		line.replace(line.find("\n"), 1, " | ");
	}

	string whole = "[" + currentTime() + "] " + line + "\n";
	while (debuglock.test_and_set(memory_order_acquire));
	ofstream relog(filename, ios_base::app);
	relog << whole;
	relog.close();
	debuglock.clear(memory_order_release);
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

#include "DebugLog.h"
#include <atomic>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

vecstr updatelog;
vecstr patchlog;
atomic_flag debuglock = ATOMIC_FLAG_INIT;

string currentTime();

void UpdateDebugOutput()
{
	if (updatelog.size() > 0)
	{
		ofstream output("UpdateLog.txt");

		for (auto& line : updatelog)
		{
			output << line << "\n";
		}

		output << "Nemesis Update Log End";
		output.close();
	}
}

void PatchDebugOutput()
{
	if (patchlog.size() > 0)
	{
		ofstream output("PatchLog.txt");

		for (auto& line : patchlog)
		{
			output << line << "\n";
		}

		output << "Nemesis Patch Log End";
		output.close();
	}
}

string currentTime()
{
	return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
}

void UpdateDebug(string line)
{
	while (debuglock.test_and_set(memory_order_acquire));
	updatelog.push_back("[" + currentTime() + "] " + line);
	debuglock.clear(memory_order_release);
}

void PatchDebug(string line)
{
	while (debuglock.test_and_set(memory_order_acquire));
	patchlog.push_back("[" + currentTime() + "] " + line);
	debuglock.clear(memory_order_release);
}

void UpdateReset()
{
	updatelog.clear();
}

void PatchReset()
{
	patchlog.clear();
}

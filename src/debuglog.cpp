#include "Global.h"

#include <boost/atomic.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "debuglog.h"

using namespace std;

vecstr updatelog;
vecstr patchlog;
boost::atomic_flag atomlock = BOOST_ATOMIC_FLAG_INIT;
;
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
        for (int i = 0; i < size; ++i)
        {
            line.replace(line.find("\n"), 1, " | ");
        }
    }

    while (atomlock.test_and_set(boost::memory_order_acquire))
        ;
    ofstream relog(filename, ios_base::app);
    relog << "[" + currentTime() + "] " + line + "\n";
    relog.close();
    atomlock.clear(boost::memory_order_release);
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

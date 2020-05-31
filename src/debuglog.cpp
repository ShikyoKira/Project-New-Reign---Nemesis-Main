#include "Global.h"
#include "debuglog.h"

#include "utilities/algorithm.h"
#include "utilities/atomiclock.h"

using namespace std;

VecStr updatelog;
VecStr patchlog;
std::atomic_flag atomlock{};
;
string filename = "CriticalLog.txt";


std::string currentTime()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return buffer;
}

std::wstring currentTimeW()
{
    return nemesis::transform_to<wstring>(currentTime());
}

void DebugOutput()
{
    filename.clear();
}

void DebugLogging(string line, bool noEndLine)
{
    int64_t size = count(line.begin(), line.end(), '\n');

    if (noEndLine)
    {
        for (int64_t i = 0; i < size; ++i)
        {
            line.replace(line.find("\n"), 1, " | ");
        }
    }

    Lockless_s lock(atomlock);
    ofstream relog(filename, ios_base::app);
    relog << "[" + currentTime() + "] " + line + "\n";
    relog.close();
}

void DebugLogging(wstring line, bool noEndLine)
{
    int64_t size = count(line.begin(), line.end(), '\n');

    if (noEndLine)
    {
        for (int64_t i = 0; i < size; ++i)
        {
            line.replace(line.find(L"\n"), 1, L" | ");
        }
    }

    Lockless_s lock(atomlock);
    wofstream relog(filename, ios_base::app);
    relog << L"[" + currentTimeW() + L"] " + line + L"\n";
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

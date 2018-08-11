#include "lastupdate.h"
#include <Windows.h>
#include <atomic>

using namespace std;

atomic_flag updateLock = ATOMIC_FLAG_INIT;

bool saveLastUpdate(string filename, unordered_map<string, string>& lastUpdate)
{
	HANDLE file;
	FILETIME lastmodified;
	SYSTEMTIME sysUTC;
	file = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		ErrorMessage(2020);
		return false;
	}

	if (!GetFileTime(file, NULL, NULL, &lastmodified))
	{
		ErrorMessage(2022);
		return false;
	}

	FileTimeToSystemTime(&lastmodified, &sysUTC);
	string time = to_string(sysUTC.wDay) + "/" + to_string(sysUTC.wMonth) + "/" + to_string(sysUTC.wYear) + " " + to_string(sysUTC.wHour) + ":" + to_string(sysUTC.wMinute);

	// boost::lock_guard<boost::mutex> fileLock(addAnimLock);		issue #61

	while (updateLock.test_and_set(memory_order_acquire));
	lastUpdate[filename] = time;
	updateLock.clear(memory_order_release);

	return true;
}

#include "lastupdate.h"
#include <atomic>
#include <time.h>

using namespace std;

atomic_flag updateLock = ATOMIC_FLAG_INIT;

bool saveLastUpdate(string filename, unordered_map<string, string>& lastUpdate)
{
	try
	{
		if (!isFileExist(filename)) ErrorMessage(2022);

		std::time_t lastmodified = boost::filesystem::last_write_time(filename);
		char time1[26];
		struct tm buf;
		localtime_s(&buf, &lastmodified);
		asctime_s(time1, sizeof time1, &buf);
		string time = time1;
		time.pop_back();

		while (updateLock.test_and_set(memory_order_acquire));
		lastUpdate[filename] = time;
		updateLock.clear(memory_order_release);
	}
	catch (...)
	{
		ErrorMessage(2022);
	}

	return true;
}

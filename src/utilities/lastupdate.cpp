#include <atomic>
#include <time.h>
#include <filesystem>

#include "utilities/lastupdate.h"
#include "utilities/atomiclock.h"

using namespace std;

atomic_flag updateLock = ATOMIC_FLAG_INIT;

bool saveLastUpdate(string filename, unordered_map<string, string>& lastUpdate)
{
	return saveLastUpdate(filename.c_str(), lastUpdate);
}

bool saveLastUpdate(string_view filename, unordered_map<string, string>& lastUpdate)
{
	return saveLastUpdate(filename.data(), lastUpdate);
}

bool saveLastUpdate(const char* filename, unordered_map<string, string>& lastUpdate)
{
	try
	{
		Lockless_s lock(updateLock);
		lastUpdate[filename] = GetLastModified(filename);
	}
	catch (exception & ex)
	{
		ErrorMessage(6001, ex.what());
	}

	return true;
}

const char* GetLastModified(const char* filename)
{
	try
	{
		struct stat buf;
		stat(filename, &buf);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &buf.st_mtime);
		buffer[24] = '\0';
		return buffer;
	}
	catch (...)
	{
	}

	return "";
}

string GetLastModified(string filename)
{
	try
	{
		struct stat buf;
		stat(filename.data(), &buf);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &buf.st_mtime);
		buffer[24] = '\0';
		return buffer;
	}
	catch (...)
	{
	}

	return "";
}

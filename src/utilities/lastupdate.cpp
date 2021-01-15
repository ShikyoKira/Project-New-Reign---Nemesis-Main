#include "Global.h"

#include <atomic>
#include <filesystem>
#include <time.h>

#include "utilities/algorithm.h"
#include "utilities/atomiclock.h"
#include "utilities/lastupdate.h"
#include <corecrt_wtime.h>

using namespace std;

atomic_flag updateLock = ATOMIC_FLAG_INIT;

void saveLastUpdate(const wstring& filepath, unordered_map<wstring, wstring>& lastUpdate)
{
    saveLastUpdate(filepath.c_str(), lastUpdate);
}

void saveLastUpdate(const wstring_view& filename, unordered_map<wstring, wstring>& lastUpdate)
{
    saveLastUpdate(nemesis::transform_to<wstring>(filename).c_str(), lastUpdate);
}

void saveLastUpdate(const string& filepath, unordered_map<wstring, wstring>& lastUpdate)
{
    saveLastUpdate(nemesis::transform_to<wstring>(filepath).c_str(), lastUpdate);
}

void saveLastUpdate(const string_view& filename, unordered_map<wstring, wstring>& lastUpdate)
{
    saveLastUpdate(nemesis::transform_to<wstring>(filename).c_str(), lastUpdate);
}

void saveLastUpdate(const wchar_t* filename, unordered_map<wstring, wstring>& lastUpdate)
{
    try
    {
        Lockless_s lock(updateLock);
        lastUpdate[filename] = GetLastModified(filename);
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
}

string GetLastModified(const string& filename)
{
    try
    {
        struct _stat64 buf;
        _stat64(filename.data(), &buf);
        char buffer[26];
        _ctime64_s(buffer, sizeof(buffer), &buf.st_mtime);
        buffer[24] = '\0';
        return buffer;
    }
    catch (...)
    {
        // ErrorMessage(2022);
    }

    return "";
}

wstring GetLastModified(const wstring& filename)
{
    try
    {
        struct _stat64 buf;
        _wstat64(filename.data(), &buf);
        wchar_t buffer[26];
        _wctime64_s(buffer, 26, &buf.st_mtime);
        buffer[24] = '\0';
        return buffer;
    }
    catch (...)
    {
        // ErrorMessage(2022);
    }

    return L"";
}

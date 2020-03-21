#include <algorithm>
#include <cctype>
#include <string>

#include "algorithm.h"

using namespace std;

namespace nemesis
{
	const char* to_lower_copy(const char* data)
	{
		char* temp = _strdup(data);
		int size = strlen(data);
		int i = 0;

		while (i < size)
		{
			temp[i] = tolower(temp[i]);
			i++;
		}

		return temp;
	}

	const wchar_t* to_lower_copy(const wchar_t* data)
	{
		wchar_t* temp = _wcsdup(data);
		int size = wcslen(data);
		int i = 0;

		while (i < size)
		{
			temp[i] = tolower(temp[i]);
			i++;
		}

		return temp;
	}

	string to_lower_copy(const string& data)
	{
		return to_lower_copy(data.c_str());
	}

	wstring to_lower_copy(const wstring& data)
	{
		return to_lower_copy(data.c_str());
	}

	void to_lower(string& data)
	{
		data = to_lower_copy(data.c_str());
	}

	void to_lower(wstring& data)
	{
		data = to_lower_copy(data.c_str());
	}

	bool iequals(const char* l, const char* r)
	{
		return strcmp(to_lower_copy(l), to_lower_copy(r)) == 0;
	}

	bool iequals(const wchar_t* l, const wchar_t* r)
	{
		return wcscmp(to_lower_copy(l), to_lower_copy(r)) == 0;
	}

	bool iequals(const char* l, const string& r)
	{
		return strcmp(to_lower_copy(l), to_lower_copy(r.c_str())) == 0;
	}

	bool iequals(const wchar_t* l, const wstring& r)
	{
		return wcscmp(to_lower_copy(l), to_lower_copy(r.c_str())) == 0;
	}

	bool iequals(const string& l, const char* r)
	{
		return strcmp(to_lower_copy(l.c_str()), to_lower_copy(r)) == 0;
	}

	bool iequals(const wstring& l, const wchar_t* r)
	{
		return wcscmp(to_lower_copy(l.c_str()), to_lower_copy(r)) == 0;
	}

	bool iequals(const string& l, const string& r)
	{
		return strcmp(to_lower_copy(l.c_str()), to_lower_copy(r.c_str())) == 0;
	}

	bool iequals(const wstring& l, const wstring& r)
	{
		return wcscmp(to_lower_copy(l.c_str()), to_lower_copy(r.c_str())) == 0;
	}
}

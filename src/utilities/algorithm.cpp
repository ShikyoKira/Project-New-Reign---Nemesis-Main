#include <algorithm>
#include <cctype>
#include <string>

#include "algorithm.h"

using namespace std;

namespace nemesis
{
	string to_lower_copy(string data)
	{
		transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	wstring to_lower_copy(wstring data)
	{
		transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	void to_lower(string& data)
	{
		transform(data.begin(), data.end(), data.begin(), ::tolower);
	}

	void to_lower(wstring& data)
	{
		transform(data.begin(), data.end(), data.begin(), ::tolower);
	}

	bool iequals(const char* l, const char* r)
	{
		return to_lower_copy(string(l)) == to_lower_copy(string(r));
	}

	bool iequals(const wchar_t* l, const wchar_t* r)
	{
		return to_lower_copy(wstring(l)) == to_lower_copy(wstring(r));
	}

	bool iequals(string l, string r)
	{
		return to_lower_copy(l) == to_lower_copy(r);
	}

	bool iequals(wstring l, wstring r)
	{
		return to_lower_copy(l) == to_lower_copy(r);
	}
}

#pragma once

#include <string>

namespace nemesis
{
	std::string to_lower_copy(std::string data);
	std::wstring to_lower_copy(std::wstring data);
	void to_lower(std::string& data);
	void to_lower(std::wstring& data);
	bool iequals(const char* l, const char* r);
	bool iequals(const wchar_t* l, const wchar_t* r);
	bool iequals(std::string l, std::string r);
	bool iequals(std::wstring l, std::wstring r);
}

#pragma once

#include <string>

namespace nemesis
{
	std::string to_lower_copy(const std::string& data);
	std::wstring to_lower_copy(const std::wstring& data);

	void to_lower(std::string& data);
	void to_lower(std::wstring& data);

	bool iequals(const char* l, const char* r);
	bool iequals(const wchar_t* l, const wchar_t* r);
	bool iequals(const char* l, const std::string& r);
	bool iequals(const wchar_t* l, const std::wstring& r);
	bool iequals(const std::string& l, const char* r);
	bool iequals(const std::wstring& l, const wchar_t* r);
	bool iequals(const std::string& l, const std::string& r);
	bool iequals(const std::wstring& l, const std::wstring& r);
}

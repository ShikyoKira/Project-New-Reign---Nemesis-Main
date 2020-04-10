#ifndef WSTRCONVERT_H_
#define WSTRCONVERT_H_

#include <codecvt>
#include <string>

// convert UTF-8 string to wstring
std::wstring StringToWString(const std::string& str);

// convert wstring to UTF-8 string
std::string WStringToString(const std::wstring& str);

#endif

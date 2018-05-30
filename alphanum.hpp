#ifndef ALPHANUM__HPP
#define ALPHANUM__HPP

#include <string>
#include <shlwapi.h>
#include <locale>
#include <codecvt>
#include "compute.h"

struct alphanum_less : public std::binary_function<std::string, std::string, bool>
{
	bool operator()(const std::string& left, const std::string& right) const
	{
		return lstrcmpiA(LPCSTR(left.c_str()), LPCSTR(right.c_str())) < 0;
	}
};

#endif

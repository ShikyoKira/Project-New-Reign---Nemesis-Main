#ifndef ALPHANUM__HPP
#define ALPHANUM__HPP

#include <string>
#include <shlwapi.h>
#include <locale>
#include <codecvt>

template<class Ty>
struct alphanum_less : public std::binary_function<Ty, Ty, bool>
{
	bool operator()(const Ty& left, const Ty& right) const
	{
		return StrCmpI(LPCSTR(left.c_str()), LPCSTR(right.c_str())) < 0;
	}
};

#endif

#ifndef ALPHANUM_HPP_
#define ALPHANUM_HPP_

#include <string>
#include <shlwapi.h>

struct alphanum_less
{
	bool operator() (const std::string& x, const std::string& y) const 
	{
		return lstrcmpiA(LPCSTR(x.c_str()), LPCSTR(y.c_str())) < 0;
	}

	typedef std::string first_argument_type;
	typedef std::string second_argument_type;
	typedef bool result_type;
};

#endif
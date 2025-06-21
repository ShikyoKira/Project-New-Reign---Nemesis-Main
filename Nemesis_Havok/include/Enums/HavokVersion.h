#pragma once

#include <stdexcept>

#include "Nemesis_Havok/include/Utilities/Types.h"

namespace nemesis
{
	enum HavokVersion : unsigned char
	{
		LEGACY,
		HK_660,
		HK_700,
		HK_710,
		HK_2010_1_0,
		HK_2010_2_0,
		HK_2011_1_0,
		HK_2011_2_0,
		HK_2011_3_0,
		HK_2011_3_1,
		HK_2012_1_0,
		HK_2012_2_0,
		HK_2012_2_1,
		HK_2013_1_0,
		HK_2013_1_1,
		HK_2013_2_0,
		HK_2013_2_5,
		HK_2013_3_0,
		HK_2014_1_0
	};

	nemesis::HavokVersion GetEnumVersion(const std::string& version_str);
    std::string GetStringVersion(nemesis::HavokVersion version);
}

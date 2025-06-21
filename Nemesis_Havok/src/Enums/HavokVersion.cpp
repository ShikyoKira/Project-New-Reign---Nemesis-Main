#include "Enums/HavokVersion.h"

const static UMap<std::string, nemesis::HavokVersion> VersionMap = {
    {"hk_6.6.0-r1", nemesis::HavokVersion::HK_660},
    {"hk_7.0.0-r1", nemesis::HavokVersion::HK_700},
    {"hk_7.1.0-r1", nemesis::HavokVersion::HK_710},
    {"hk_2010.1.0-r1", nemesis::HavokVersion::HK_2010_1_0},
    {"hk_2010.2.0-r1", nemesis::HavokVersion::HK_2010_2_0},
    {"hk_2011.1.0-r1", nemesis::HavokVersion::HK_2011_1_0},
    {"hk_2011.2.0-r1", nemesis::HavokVersion::HK_2011_2_0},
    {"hk_2011.3.0-r1", nemesis::HavokVersion::HK_2011_3_0},
    {"hk_2012.1.0-r1", nemesis::HavokVersion::HK_2012_1_0},
    {"hk_2012.2.0-r1", nemesis::HavokVersion::HK_2012_2_0},
    {"hk_2013.1.0-r1", nemesis::HavokVersion::HK_2013_1_0},
    {"hk_2013.2.0-r1", nemesis::HavokVersion::HK_2013_2_0},
    {"hk_2013.3.0-r1", nemesis::HavokVersion::HK_2013_3_0},
};

nemesis::HavokVersion nemesis::GetEnumVersion(const std::string& version_str)
{
    auto itr = VersionMap.find(version_str);

    if (itr != VersionMap.end()) return itr->second;
   
    throw std::runtime_error(version_str + " havok version is not supported");
}

std::string nemesis::GetStringVersion(nemesis::HavokVersion version)
{
    for (auto& each : VersionMap)
    {
        if (each.second != version) continue;

        return each.first;
    }

    throw std::runtime_error(std::to_string(version) + " havok version is not supported");
}

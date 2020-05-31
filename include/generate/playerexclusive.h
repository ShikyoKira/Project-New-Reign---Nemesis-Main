#ifndef PLAYEREXCLUSIVE_H_
#define PLAYEREXCLUSIVE_H_

class NemesisInfo;

// PCEA data
struct PCEA
{
    std::string modFile;                                       // mod folder name
    std::unordered_map<std::string, std::string> animPathList; // animation file, animation path
};

// PCEA data
struct PCEAData
{
    std::string modFile;  // mod folder name
    std::string animPath; // animation path
};

extern std::vector<PCEA> pcealist; // list of mods with pcea
extern std::unordered_map<std::string, std::vector<PCEAData>>
    animReplaced; // animation file, list of ordered mod + animation path

void ReadPCEA(const NemesisInfo* nemesisInfo);
bool PCEAInstallation(const NemesisInfo* nemesisInfo);

#endif

#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

#include <filesystem>

struct NemesisInfo;

extern std::unordered_map<std::string, int> AAgroup_Counter;

void AAInitialize(std::string AAList);
bool AAInstallation(const NemesisInfo* nemesisInfo);

bool FolderCreate(std::string curBehaviorPath);
bool FolderCreate(std::wstring curBehaviorPath);
unsigned int CRC32Convert(std::string line);
unsigned int CRC32Convert(std::wstring line);
bool PapyrusCompile(std::filesystem::path pscfile,
                    std::filesystem::path import,
                    std::filesystem::path destination,
                    std::filesystem::path filepath,
                    std::filesystem::path appdata_path,
                    std::filesystem::path nemesisInfo);

#endif

#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

#include <filesystem>

struct NemesisInfo;

extern std::unordered_map<std::string, int> AAgroup_Counter;

void AAInitialize(std::string AAList);
bool AAInstallation(const NemesisInfo* nemesisInfo);

bool FolderCreate(std::string curBehaviorPath);
bool FolderCreate(std::wstring curBehaviorPath);
uint CRC32Convert(std::string line);
bool PapyrusCompile(std::filesystem::path pscfile,
                    std::wstring import,
                    std::string destination,
                    std::string filepath,
                    std::filesystem::path appdata_path,
                    std::filesystem::path nemesisInfo);

#endif
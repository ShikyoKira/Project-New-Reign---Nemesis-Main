#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

#include <filesystem>

class NemesisInfo;

extern std::unordered_map<std::string, int> AAgroup_Counter;

void AAInitialize(std::string AAList);
bool AAInstallation(const NemesisInfo* nemesisInfo);

size_t CRC32Convert(std::string line);
size_t CRC32Convert(std::wstring line);

#endif

#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

extern std::unordered_map<std::string, int> AAgroup_Counter;

void AAInitialize(std::string AAList);
bool AAInstallation();

std::string GetLastModified(std::string filename);
bool FolderCreate(std::string curBehaviorPath);
unsigned int CRC32Convert(std::string line);
bool PapyrusCompile(std::string pscfile, std::string import, std::string destination, std::string filepath, std::string appdata_path);

#endif
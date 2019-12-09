#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

extern std::unordered_map<std::string, int> AAgroup_Counter;

void AAInitialize(std::string AAList);
bool AAInstallation();

std::string GetLastModified(std::string filename);
bool FolderCreate(std::string curBehaviorPath);
bool FolderCreate(std::wstring curBehaviorPath);
unsigned int CRC32Convert(std::string line);
bool PapyrusCompile(boost::filesystem::path pscfile, std::wstring import, std::string destination, std::string filepath, boost::filesystem::path appdata_path);

#endif
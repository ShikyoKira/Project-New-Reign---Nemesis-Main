#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

void AAInitialize(std::string AAList);
bool AAInstallation();

std::string GetLastModified(std::string filename);
void FolderCreate(std::string curBehaviorPath);
unsigned int CRC32Convert(std::string line);
bool PapyrusCompile(std::string pscfile, std::string import, std::string destination, std::string filepath);

#endif
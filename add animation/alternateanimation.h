#ifndef ALTERNATEANIMATION_H_
#define ALTERNATEANIMATION_H_

extern void AAInitialize(std::string AAList);
extern void AAInstallation();

extern std::string GetLastModified(std::string filename);
extern void FolderCreate(std::string curBehaviorPath);
extern unsigned int CRC32Convert(std::string line);

#endif
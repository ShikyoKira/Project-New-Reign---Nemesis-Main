#ifndef LASTUPDATE_H_
#define LASTUPDATE_H_

bool saveLastUpdate(std::string filename, std::unordered_map<std::string, std::string>& lastUpdate);
bool saveLastUpdate(std::string_view filename, std::unordered_map<std::string, std::string>& lastUpdate);
bool saveLastUpdate(const char* filename, std::unordered_map<std::string, std::string>& lastUpdate);

const char* GetLastModified(const char* filename);
std::string GetLastModified(std::string filename);

#endif

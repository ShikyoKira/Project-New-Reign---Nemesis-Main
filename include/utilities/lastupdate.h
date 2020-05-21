#ifndef LASTUPDATE_H_
#define LASTUPDATE_H_

void saveLastUpdate(const std::string& filename, std::unordered_map<std::string, std::string>& lastUpdate);
void saveLastUpdate(const std::string_view& filename,
                    std::unordered_map<std::string, std::string>& lastUpdate);
void saveLastUpdate(const char* filename, std::unordered_map<std::string, std::string>& lastUpdate);

std::string GetLastModified(std::string filename);

#endif

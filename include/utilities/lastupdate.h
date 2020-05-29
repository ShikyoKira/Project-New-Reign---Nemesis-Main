#ifndef LASTUPDATE_H_
#define LASTUPDATE_H_

void saveLastUpdate(const std::string& filename, std::unordered_map<std::wstring, std::wstring>& lastUpdate);
void saveLastUpdate(const std::string_view& filename,
                    std::unordered_map<std::wstring, std::wstring>& lastUpdate);
void saveLastUpdate(const std::wstring& filename, std::unordered_map<std::wstring, std::wstring>& lastUpdate);
void saveLastUpdate(const std::wstring_view& filename,
                    std::unordered_map<std::wstring, std::wstring>& lastUpdate);
void saveLastUpdate(const wchar_t* filename, std::unordered_map<std::wstring, std::wstring>& lastUpdate);

std::string GetLastModified(std::string filename);
std::wstring GetLastModified(std::wstring filename);

#endif

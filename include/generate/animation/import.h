#ifndef IMPORT_H_
#define IMPORT_H_

#include <string>
#include <vector>

typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

VecStr importOutput(std::vector<ImportContainer>& ExportID, int counter, int nextID);

#endif

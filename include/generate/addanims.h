#ifndef ADDANIMS_H_
#define ADDANIMS_H_

#include <string>
#include <unordered_map>
#include <vector>

bool AddAnims(std::string& line,
              const std::string& _animPath,
              const std::wstring& outputdir,
              const std::string& behaviorFile,
              const std::string& lowerBehaviorFile,
              const std::string& _newMod,
              std::vector<std::string>& storeline,
              int& counter,
              std::unordered_map<std::string, bool>& isAdded,
              bool& addAnim);

#endif
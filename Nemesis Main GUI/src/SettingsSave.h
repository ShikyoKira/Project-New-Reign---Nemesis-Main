#ifndef SETTINGSSAVE_H_
#define SETTINGSSAVE_H_

#include <vector>
#include <string>
#include <unordered_map>

void createCache(std::string, std::vector<std::string> chosenBehavior);
bool getCache(std::string& language, std::unordered_map<std::string, bool>& chosenBehavior);

#endif

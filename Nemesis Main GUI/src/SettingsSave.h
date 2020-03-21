#ifndef SETTINGSSAVE_H_
#define SETTINGSSAVE_H_

#include <vector>
#include <string>
#include <unordered_map>

void createLanguageCache(std::string language);
void createModCache(std::vector<std::string> chosenBehavior);
void createModOrderCache(std::vector<std::string> behaviorList);

bool getCache(std::string& language, std::unordered_map<std::string, bool>& chosenBehavior);
bool getCache(std::wstring& language, std::unordered_map<std::string, bool>& chosenBehavior);

bool getOrderCache(std::vector<std::string>& orderList);

#endif

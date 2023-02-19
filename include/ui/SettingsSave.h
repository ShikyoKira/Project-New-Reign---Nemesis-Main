#ifndef SETTINGSSAVE_H_
#define SETTINGSSAVE_H_

#include <string>
#include <unordered_map>
#include <vector>

void CreateLanguageCache(const std::wstring& language);
void CreateModCache(const VecStr& chosenBehavior);
void CreateModOrderCache(const VecStr& behaviorList);

std::wstring GetCachedLanguage();

bool GetModSelectionCache(UMap<std::string, bool>& chosenBehavior);

bool GetOrderCache(VecWstr& order_list);

#endif

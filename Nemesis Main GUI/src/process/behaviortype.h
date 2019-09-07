#ifndef BEHAVIORTYPE_H_
#define BEHAVIORTYPE_H_

#include "Global.h"


typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> SSSMap;
typedef std::unordered_map<std::string, std::set<std::string>> mapSetString;
typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, std::set<std::string>>>> StateIDList;
typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, bool>>> MapChildState;

#endif
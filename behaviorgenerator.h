#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

#include <string>

typedef std::unordered_map<std::string, std::set<std::string>> mapSetString;

bool hkxcmdProcess(std::string xmlfile, std::string hkxfile);
bool hkxcmdXmlInput(std::string hkxfile, vecstr& fileline);

#endif
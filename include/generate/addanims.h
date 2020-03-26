#ifndef ADDANIMS_H_
#define ADDANIMS_H_

#include <string>
#include <vector>
#include <unordered_map>

bool AddAnims(std::string& line, std::string animPath, std::string outputdir, std::string behaviorFile, std::string lowerBehaviorFile, std::string& newMod,
	std::vector<std::string>& storeline, int& counter, std::unordered_map<std::string, bool>& isAdded, bool& addAnim);

#endif
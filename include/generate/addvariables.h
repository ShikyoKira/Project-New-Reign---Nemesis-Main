#ifndef ADDVARIABLES_H_
#define ADDVARIABLES_H_

bool AddVariables(std::string curNum, vecstr& storeline, std::string variablename, std::unordered_map<std::string, bool> orivariable, std::unordered_map<std::string, bool>& isExist,
	int& counter, std::string ZeroVariable, std::unordered_map<std::string, int>& variableid, std::unordered_map<int, std::string>& varName, bool isInt = true);

#endif

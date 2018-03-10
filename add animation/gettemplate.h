#ifndef GETTEMPLATE_H_
#define GETTEMPLATE_H_

#include "optionlist.h"

struct getTemplate
{
	std::unordered_map<std::string, OptionList> optionlist;
	std::unordered_map<std::string, vecstr> grouplist;
	std::unordered_map<std::string, bool> templatelist;
	std::unordered_map<std::string, vecstr> behaviortemplate;
	std::unordered_map<std::string, std::vector<int>> existingFunctionID;
	std::unordered_map<std::string, vecstr> templatefilelist;
	std::unordered_map<std::string, int> mainBehaviorJoint;

	getTemplate();
};


#endif
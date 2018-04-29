#ifndef REGISTERANIMATION_H_
#define REGISTERANIMATION_H_

#include <string>
#include <iostream>
#include "optionlist.h"
#include "gettemplate.h"
#include "animationinfo.h"

struct var
{
	std::string var_type = "BOOL";
	std::string init_value = "0";

	var(std::string inputtype, std::string inputvalue);
	var() {}
};

struct registerAnimation
{
public:
	std::string modID;
	std::string version;
	std::string behaviorFile;
	std::unordered_map<std::string, var> AnimVar;
	std::unordered_map<std::string, int> templateType;
	std::unordered_map<std::string, std::vector<int>> last;
	std::unordered_map<std::string, std::vector<int>> isMulti;
	std::unordered_map<std::string, std::vector<std::shared_ptr<animationInfo>>> animInfo;

	registerAnimation(std::string filepath, std::string shortfilepath, getTemplate behaviortemplate, std::string behavior, std::string behaviorfile, bool isNemesis = false);
	void clear();
};

#endif
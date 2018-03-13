#ifndef ANIMATIONINFO_H_
#define ANIMATIONINFO_H_

#include "optionlist.h"

#pragma warning(disable:4503)

typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct animationInfo
{
	bool hasDuration = false;
	double duration = 0;
	std::string filename;
	std::string mainAnimEvent;
	vecstr eventID;
	vecstr animInfo;
	vecstr variableID;
	SSMap mixOptRegis;
	ImportContainer addition;
	std::unordered_map<std::string, vecstr> addOn;
	std::unordered_map<std::string, vecstr> mixOptRever;
	std::unordered_map<std::string, vecstr> linkedOption;
	std::unordered_map<std::string, bool> optionPicked;
	std::unordered_map<std::string, int> optionPickedCount;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition;
	std::unordered_map<int, std::string> AnimObject;

	animationInfo(vecstr newAnimInfo, std::string curFilename, OptionList behaviorOption, int linecount, bool& isOExist, bool noOption = false);
	void addFilename(std::string curFilename);
	void storeAnimObject(vecstr animobjects, std::string listFilename, int lineCount);
	void groupAdditionProcess(std::string header, std::string addOnName, std::string name, std::unordered_map<std::string, bool> optionGroup);
};

#endif
#ifndef OPTIONLIST_H_
#define OPTIONLIST_H_

#include <string>
#include <vector>
#include <unordered_map>

typedef std::vector<std::string> vecstr;

struct OptionList
{
	bool core = false;
	bool ignoreGroup = false;

	int groupMin = -1;
	int animObjectCount = 0;

	std::string coreBehavior;
	std::string startStateID;
	std::string templatecode;

	vecstr ruleOne;
	vecstr ruleTwo;
	vecstr compulsory;
	vecstr optionOrder;

	std::unordered_map<std::string, std::unordered_map<int, int>> multiState;		// behavior, state number, node/function ID
	std::unordered_map<std::string, bool> storelist;
	std::unordered_map<std::string, bool> groupOption;
	std::unordered_map<std::string, std::string> mixOptRegis;
	std::unordered_map<std::string, vecstr> mixOptRever;
	std::unordered_map<std::string, vecstr> joint;
	std::unordered_map<std::string, vecstr> addOn;											// option, list of add-on
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> modAddOn;	// option, addon, modifier; use to modify add on, not always needed but can be useful especially for event/variable

	std::vector<vecstr> eleEvent;
	vecstr eleEventLine;
	std::vector<vecstr> eleEventGroupF;
	vecstr eleEventGroupFLine;
	std::vector<vecstr> eleEventGroupL;
	vecstr eleEventGroupLLine;
	std::vector<vecstr> eleVar;
	vecstr eleVarLine;
	std::vector<vecstr> eleVarGroupF;
	vecstr eleVarGroupFLine;
	std::vector<vecstr> eleVarGroupL;
	vecstr eleVarGroupLLine;

	OptionList(std::string filepath, std::string format);
	OptionList();
	void OptionList::setDebug(bool isDebug);
};

bool optionMatching(std::string option1, std::string option2);

#endif
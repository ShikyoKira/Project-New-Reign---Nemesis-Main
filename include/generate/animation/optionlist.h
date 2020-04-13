#ifndef OPTIONLIST_H_
#define OPTIONLIST_H_

#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::string> VecStr;

struct OptionList
{
    bool core        = false;
    bool ignoreGroup = false;

    int groupMin        = -1;
    int animObjectCount = 0;

    std::string coreBehavior;
    std::string startStateID;
    std::string templatecode;

    VecStr ruleOne;
    VecStr ruleTwo;
    VecStr compulsory;
    VecStr optionOrder;

    std::unordered_map<std::string, std::unordered_map<int, int>>
        multiState; // behavior, state number, node/function ID
    std::unordered_map<std::string, bool> storelist;
    std::unordered_map<std::string, bool> groupOption;
    std::unordered_map<std::string, std::string> mixOptRegis;
    std::unordered_map<std::string, VecStr> mixOptRever;
    std::unordered_map<std::string, VecStr> joint;
    std::unordered_map<std::string, VecStr> addOn; // option, list of add-on
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
        modAddOn; // option, addon, modifier; use to modify add on, not always needed but can be useful especially for event/variable

    std::vector<VecStr> eleEvent;
    VecStr eleEventLine;
    std::vector<VecStr> eleEventGroupF;
    VecStr eleEventGroupFLine;
    std::vector<VecStr> eleEventGroupL;
    VecStr eleEventGroupLLine;
    std::vector<VecStr> eleVar;
    VecStr eleVarLine;
    std::vector<VecStr> eleVarGroupF;
    VecStr eleVarGroupFLine;
    std::vector<VecStr> eleVarGroupL;
    VecStr eleVarGroupLLine;

    OptionList(std::string filepath, std::string format);
    OptionList();
    void setDebug(bool isDebug);
};

bool optionMatching(std::string option1, std::string option2);

#endif

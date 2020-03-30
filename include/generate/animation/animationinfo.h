#ifndef ANIMATIONINFO_H_
#define ANIMATIONINFO_H_

#include "generate/animation/optionlist.h"

#pragma warning(disable : 4503)

typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct AnimationInfo
{
    bool ignoreGroup          = false;
    bool known                = false;
    bool hasDuration          = false;
    int animObjectCount       = 0;
    double duration           = 0;
    std::string filename      = "";
    std::string mainAnimEvent = "";
    // vecstr clipname;		obsolete
    vecstr eventID;
    vecstr animInfo;
    vecstr variableID;
    vecstr motionData;
    vecstr rotationData;
    SSMap mixOptRegis;
    ImportContainer addition;
    std::unordered_map<std::string, vecstr> addOn; // option, list of add-on
    std::unordered_map<std::string, vecstr> mixOptRever;
    std::unordered_map<std::string, vecstr> linkedOption;
    std::unordered_map<std::string, bool> groupOption;
    std::unordered_map<std::string, bool> optionPicked;
    std::unordered_map<std::string, int> optionPickedCount;
    std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition;
    std::unordered_map<int, vecstr> AnimObject;

    AnimationInfo()
    {}
    AnimationInfo(vecstr newAnimInfo,
                  std::string curFilename,
                  OptionList behaviorOption,
                  int linecount,
                  bool& isOExist,
                  bool noOption = false);
    void addFilename(std::string curFilename);
    void storeAnimObject(vecstr animobjects, std::string listFilename, int lineCount);
    void groupAdditionProcess(
        std::string header,
        std::string addOnName,
        std::string name,
        std::unordered_map<std::string, bool> optionGroup,
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> modAddOn);
};

#endif

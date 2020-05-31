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
    // VecStr clipname;		obsolete
    VecStr eventID;
    VecStr animInfo;
    VecStr variableID;
    VecStr motionData;
    VecStr rotationData;
    SSMap mixOptRegis;
    ImportContainer addition;
    std::unordered_map<std::string, VecStr> addOn; // option, list of add-on
    std::unordered_map<std::string, VecStr> mixOptRever;
    std::unordered_map<std::string, VecStr> linkedOption;
    std::unordered_map<std::string, bool> groupOption;
    std::unordered_map<std::string, bool> optionPicked;
    std::unordered_map<std::string, int> optionPickedCount;
    std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> groupAddition;
    std::unordered_map<int, VecStr> AnimObject;

    AnimationInfo()
    {}
    AnimationInfo(VecStr newAnimInfo,
                  std::filesystem::path curFilename,
                  OptionList behaviorOption,
                  int linecount,
                  bool& isOExist,
                  bool noOption = false);
    void addFilename(std::string curFilename);
    void storeAnimObject(VecStr animobjects, std::filesystem::path listFilename, int lineCount);
    void groupAdditionProcess(
        std::string header,
        std::string addOnName,
        std::string name,
        std::unordered_map<std::string, bool> optionGroup,
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> modAddOn);
};

#endif

#ifndef REGISTERANIMATION_H_
#define REGISTERANIMATION_H_

#include <iostream>
#include <string>

#include "generate/animation/animationinfo.h"
#include "generate/animation/templateinfo.h"

struct var
{
    std::string var_type   = "BOOL";
    std::string init_value = "0";

    var(std::string inputtype, std::string inputvalue, bool& failed);
    var()
    {}
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
    std::unordered_map<std::string, std::vector<std::shared_ptr<AnimationInfo>>>
        animInfo; // template code, list of animinfo

    registerAnimation(std::string filepath,
                      std::string shortfilepath,
                      TemplateInfo behaviortemplate,
                      std::string behavior,
                      std::string behaviorfile,
                      bool fstP,
                      bool isNemesis = false);
    void clear();
};

#endif
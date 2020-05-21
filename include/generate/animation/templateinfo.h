#ifndef TEMPLATEINFO_H_
#define TEMPLATEINFO_H_

#include <map>
#include <unordered_set>

#include "utilities/alphanum.hpp"

typedef std::set<std::string> SetStr;
typedef std::unordered_set<std::string> USetStr;

struct OptionList;

struct TemplateInfo
{
    std::unordered_map<std::string, OptionList>
        optionlist; // animation code, option list; access point to option list from animation code
    std::unordered_map<std::string, SetStr>
        grouplist; // behavior file, list of animation code(in vector of string); assign animation to their respective behavior
    std::unordered_map<std::string, bool> templatelist; // animation code, true/false; register template

    // Behavior
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>
        existingFunctionID; // animation code, behavior, list of node ID(in vector of int); node(function) from the master branch that is/are used in the generation of animation
    std::unordered_map<std::string, std::unordered_map<std::string, std::map<int, int>>>
        mainBehaviorJoint; // animation code, behavior, state number, node(function) ID; record the node that is joint with the master branch
    std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>
        behaviortemplate; // animation code, behavior, template file(in vector of string); store template in memory
    std::unordered_map<std::string, std::string>
        coreTemplate; // animation code, core behavior; animation code that uses core behavior
    std::unordered_map<std::string, std::string>
        coreBehaviorCode; // core behavior, proxy animation code; get the proxy animation code using the file name

    // AnimData
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>>>
        asdtemplate; // animation code, project, header, template file; animationsetdatasinglefile template by header
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>>
        animdatatemplate; // animation code, character, header, template file; animationdatasinglefile template by header
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>
        existingASDHeader; // animation code, project, list of header; header from the master branch that is/are used in the generation of animation
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>
        existingAnimDataHeader; // animation code, character, list of header; header from the master branch that is/are used in the generation of animation

    TemplateInfo();
};

#endif

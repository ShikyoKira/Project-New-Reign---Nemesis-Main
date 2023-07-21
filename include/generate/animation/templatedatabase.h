#ifndef TEMPLATEINFO_H_
#define TEMPLATEINFO_H_

#include "utilities/types.h"
#include "utilities/alphanum.hpp"


struct OptionList;

struct TemplateDataBase
{
    // animation code, option list; access point to option list from animation code
    UMap<std::string, OptionList> optionlist;

    // behavior file, list of animation code(in vector of string); assign animation to their respective behavior
    UMap<std::string, SetStr> grouplist;

    // animation code, true/false; register template
    UMap<std::string, bool> templatelist;

    // Behavior
    // animation code, behavior, list of node ID(in vector of int); node(function) from the master branch that is/are used in the generation of animation
    UMap<std::string, UMap<std::string, Vec<int>>> existingFunctionID;

    // animation code, behavior, state number, node(function) ID; record the node that is joint with the master branch
    UMap<std::string, UMap<std::string, Map<int, int>>> mainBehaviorJoint;

    // animation code, behavior, template file(in vector of string); store template in memory
    UMap<std::string, UMap<std::string, VecStr>> behaviortemplate;

    // animation code, core behavior; animation code that uses core behavior
    UMap<std::string, std::string> coreTemplate;

    // core behavior, proxy animation code; get the proxy animation code using the file name
    UMap<std::string, std::string> coreBehaviorCode;

    // AnimData
    // animation code, project, header, template file; animationsetdatasinglefile template by header
    UMap<std::string, UMap<std::string, Map<std::string, VecStr, alphanum_less>>> asdtemplate;

    // animation code, character, header, template file; animationdatasinglefile template by header
    UMap<std::string, UMap<std::string, UMap<std::string, VecStr>>> animdatatemplate;

    // animation code, project, list of header; header from the master branch that is/are used in the generation of animation
    UMap<std::string, UMap<std::string, USet<std::string>>> existingASDHeader;

    // animation code, character, list of header; header from the master branch that is/are used in the generation of animation
    UMap<std::string, UMap<std::string, USet<std::string>>> existingAnimDataHeader;

    TemplateDataBase();
};

#endif

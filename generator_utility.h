#pragma once
#include <boost\regex.hpp>
#include "add animation\registeranimation.h"
#include "add animation\gettemplate.h"
#include "skyrimdirectory.h"

extern int GetStateID(int mainJoint, std::map<int, vecstr> functionlist);
extern int GetStateCount(vecstr templatelines);
extern vecstr newAnimationElement(std::string line, std::vector<vecstr> element, int curNumber);
extern std::string behaviorLineChooser(std::string originalline, std::unordered_map<std::string, std::string> chosenLines, vecstr behaviorPriority);
extern std::vector<std::unique_ptr<registerAnimation>> openFile(getTemplate behaviortemplate);

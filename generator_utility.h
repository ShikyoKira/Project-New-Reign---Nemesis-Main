#pragma once
#include <boost\regex.hpp>
#include "add animation\registeranimation.h"
#include "add animation\gettemplate.h"
#include "skyrimdirectory.h"
#include "add animation\furniture.h"
#include "add animation\alternateanimation.h"

extern inline bool isEdited(getTemplate& BehaviorTemplate, std::string& lowerBehaviorFile, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, bool isCharacter);
extern std::vector<int> GetStateID(std::map<int, int> mainJoint, std::map<int, vecstr> functionlist);
extern int GetStateCount(vecstr templatelines);
extern vecstr newAnimationElement(std::string line, std::vector<vecstr> element, int curNumber);
extern std::string behaviorLineChooser(std::string originalline, std::unordered_map<std::string, std::string> chosenLines, vecstr behaviorPriority);
extern std::string GetFileName(std::string filepath);
extern std::vector<std::unique_ptr<registerAnimation>> openFile(getTemplate behaviortemplate);

extern bool isEngineUpdated();
extern void ClearGlobal();
extern void GetAnimData();
extern void characterHKX();
extern void GetBehaviorPath();
extern void GetBehaviorProject();
extern void FolderCreate(std::string curBehaviorPath);

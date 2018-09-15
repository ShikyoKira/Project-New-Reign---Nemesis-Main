#pragma once
#include <boost\regex.hpp>
#include "add animation\registeranimation.h"
#include "add animation\gettemplate.h"
#include "skyrimdirectory.h"
#include "add animation\furniture.h"
#include "add animation\alternateanimation.h"

bool isEdited(getTemplate& BehaviorTemplate, std::string& lowerBehaviorFile, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, bool isCharacter, std::string modID);
bool newAnimSkip(std::vector<std::shared_ptr<Furniture>> newAnim, std::string modID);
bool GetStateCount(std::vector<int>& count, vecstr templatelines, std::string format, std::string filename, bool hasGroup);
std::vector<int> GetStateID(std::map<int, int> mainJoint, std::map<int, vecstr> functionlist, std::unordered_map<int, int>& functionState);
vecstr newAnimationElement(std::string line, std::vector<vecstr> element, int curNumber);
std::string behaviorLineChooser(std::string originalline, std::unordered_map<std::string, std::string> chosenLines, vecstr behaviorPriority);
std::string GetFileName(std::string filepath);
std::string GetFileDirectory(std::string filepath);
std::vector<std::unique_ptr<registerAnimation>> openFile(getTemplate behaviortemplate);

bool isEngineUpdated();
void ClearGlobal(bool all = true);
void GetAnimData();
void characterHKX();
void GetBehaviorPath();
void GetBehaviorProject();
void GetBehaviorProjectPath();

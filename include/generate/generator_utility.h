#ifndef GENERATOR_UTILITY_H_
#define GENERATOR_UTILITY_H_

#include <boost/regex.hpp>

#include "nemesisinfo.h"

#include "generate/alternateanimation.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/registeranimation.h"

typedef std::vector<std::string> vecstr;

bool isEdited(TemplateInfo* BehaviorTemplate,
              std::string& lowerBehaviorFile,
              std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>>& newAnimation,
              bool isCharacter,
              std::string modID);
bool newAnimSkip(std::vector<std::shared_ptr<NewAnimation>> newAnim, std::string modID);
bool GetStateCount(
    std::vector<int>& count, vecstr templatelines, std::string format, std::string filename, bool hasGroup);
int getTemplateNextID(vecstr& templatelines);
std::vector<int> GetStateID(std::map<int, int> mainJoint,
                            std::map<int, vecstr> functionlist,
                            std::unordered_map<int, int>& functionState);
vecstr newAnimationElement(std::string line, std::vector<vecstr> element, int curNumber);
std::string behaviorLineChooser(std::string originalline,
                                std::unordered_map<std::string, std::string> chosenLines,
                                vecstr behaviorPriority);

std::string GetFileName(std::string filepath);
std::string GetFileName(std::string_view filepath);
std::wstring GetFileName(std::wstring filepath);
std::wstring GetFileName(std::wstring_view filepath);

std::string GetFileDirectory(std::string filepath);
std::wstring GetFileDirectory(std::wstring filepath);

std::vector<std::unique_ptr<registerAnimation>> openFile(TemplateInfo* behaviortemplate);
void checkClipAnimData(std::string& line, vecstr& characterFiles, std::string& clipName, bool& isClip);
void checkAllFiles(std::string filepath);
void checkAllStoredHKX();

bool isEngineUpdated(std::string& versionCode);
void ClearGlobal(bool all = true);
void GetAnimData();
void characterHKX();
void GetBehaviorPath();
void GetBehaviorProject();
void GetBehaviorProjectPath();

#endif
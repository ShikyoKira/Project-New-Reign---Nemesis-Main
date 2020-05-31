#ifndef GENERATOR_UTILITY_H_
#define GENERATOR_UTILITY_H_


#include "nemesisinfo.h"

#include "generate/alternateanimation.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/registeranimation.h"

#include "utilities/regex.h"

typedef std::vector<std::string> VecStr;

class BehaviorStart;
class NemesisInfo;

bool isEdited(TemplateInfo* BehaviorTemplate,
              std::string& lowerBehaviorFile,
              std::unordered_map<std::string, std::vector<std::shared_ptr<NewAnimation>>>& newAnimation,
              bool isCharacter,
              std::string modID);
bool newAnimSkip(std::vector<std::shared_ptr<NewAnimation>> newAnim, std::string modID);
void checkBehaviorJoint(std::filesystem::path filename,
                        std::filesystem::path projectdir,
                        std::string& line,
                        BehaviorStart* process,
                        bool& isBehavior);
bool GetStateCount(
    std::vector<int>& count, VecStr templatelines, std::string format, std::string filename, bool hasGroup);
int getTemplateNextID(VecStr& templatelines);
std::vector<int> GetStateID(std::map<int, int> mainJoint,
                            std::map<int, VecStr> functionlist,
                            std::unordered_map<int, int>& functionState);
VecStr newAnimationElement(std::string line, std::vector<VecStr> element, int curNumber);
std::string behaviorLineChooser(const std::string& originalline,
                                const std::unordered_map<std::string, std::string>& chosenLines,
                                const VecStr& behaviorPriority);

std::string GetFileName(std::string filepath);
std::string GetFileName(std::string_view filepath);
std::wstring GetFileName(std::wstring filepath);
std::wstring GetFileName(std::wstring_view filepath);

std::string GetFileDirectory(std::string filepath);
std::wstring GetFileDirectory(std::wstring filepath);

bool FolderCreate(std::filesystem::path curBehaviorPath);

std::vector<std::unique_ptr<registerAnimation>> openFile(TemplateInfo* behaviortemplate,
                                                         const NemesisInfo* nemesisInfo);
void checkClipAnimData(std::filesystem::path filename,
                       std::filesystem::path projectdir,
                       std::string& line,
                       VecStr& characterFiles,
                       std::string& clipName,
                       bool& isClip,
                       BehaviorStart* process,
                       bool& isBehavior);
void checkAllFiles(std::filesystem::path filepath);
void checkAllStoredHKX();

bool isEngineUpdated(std::string& versionCodem, const NemesisInfo* nemesisInfo);
void ClearGlobal(bool all = true);
void GetAnimData();
void characterHKX();
void GetBehaviorPath();
void GetBehaviorProject();
void GetBehaviorProjectPath();

#endif
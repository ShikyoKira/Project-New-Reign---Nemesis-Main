#ifndef FURNITURE_H_
#define FURNITURE_H_

#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "generate/AnimationUtility.h"

#include "utilities/alphanum.hpp"
#include "utilities/regex.h"

#include "generate/animation/animationthread.h"
#include "generate/animation/registeranimation.h"

typedef std::vector<std::string> VecStr;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;
typedef std::unordered_map<std::string, int> ID;

class AnimTemplate;
struct condset;
struct proc;

struct MDException
{};

class NewAnimation
{
private:
    std::string zeroEvent;
    std::string zeroVariable;

    std::string behaviorFile;
    std::string filepath;
    std::string filename;
    std::string format;
    std::string strID;

    // VecStr clipname;		obsolete
    VecStr eventID;
    VecStr variableID;
    SSMap IDExist;
    SSMap mixOptRegis;
    ImportContainer* newImport;
    NewAnimLock* atomicLock;
    ImportContainer addition;

    bool isLastOrder  = false;
    bool hasDuration  = false;
    bool isEnd        = false;
    bool elementCatch = false;
    bool known;

    size_t templatesize;
    int furnitureCount = 0;
    int nextFunctionID;
    int order       = 0;
    int lastOrder   = 0;
    double duration = 0;

    std::unordered_map<int, VecStr> AnimObject;
    std::unordered_map<std::string, bool> optionPicked;
    std::unordered_map<std::string, int> optionPickedCount;
    std::unordered_map<std::string, VecStr> addOn;
    std::unordered_map<std::string, VecStr> linkedOption;
    std::unordered_map<std::string, VecStr> mixOptRever;
    std::unordered_map<std::string, VecStr> rawtemplate;
    //std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>> asdlines;
    //std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> animdatalines;
    std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> groupAddition;
    std::vector<std::shared_ptr<AnimationInfo>> groupAnimInfo;

    std::shared_ptr<std::unordered_map<std::string, AnimTemplate>> animtemplate;
    std::unordered_map<std::string, std::unordered_map<std::string, AnimTemplate>> animdatatemplate;
    std::unordered_map<std::string, std::map<std::string, AnimTemplate, alphanum_less>> asdtemplate;

public:
    std::shared_ptr<single> subFunctionIDs;
    std::string mainAnimEvent;
    std::string coreModID;

private:
    bool singleCondition(std::string condition,
                         VecStr& storeline,
                         std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                         int numline,
                         AnimationUtility utility);
    bool andLoop(std::string condition,
                 VecStr& storeline,
                 std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                 int numline,
                 AnimationUtility utility);
    bool andOrParenthesis(size_t c_and,
                          size_t c_or,
                          std::string condition,
                          VecStr& storeline,
                          std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                          int numline,
                          AnimationUtility utility);
    bool andParenthesis(std::string condition,
                        VecStr& storeline,
                        std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                        int numline,
                        AnimationUtility utility);
    bool newCondition(std::string condition,
                      VecStr& storeline,
                      std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                      int numline,
                      AnimationUtility utility);
    bool specialCondition(std::string condition, VecStr& storeline, int linecount, AnimationUtility utility);
    bool GetFirstCondition(std::string firstCondition,
                           VecStr optionInfo,
                           int numline,
                           std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                           bool isNot);
    bool conditionProcess(std::string condition,
                          std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                          bool isNot,
                          int numline,
                          AnimationUtility utility);
    void stateReplacer(std::string& line,
                       std::string statenum,
                       int stateID,
                       int linecount,
                       std::string otherAnimOrder = "",
                       bool otherAnim             = false);
    bool addOnConverter(int& optionMulti,
                        int& endMulti,
                        VecStr& optionInfo,
                        std::shared_ptr<AnimationInfo> animInfo,
                        bool second);
    void processing(std::string& newline,
                    VecStr& storeline,
                    std::string masterFormat,
                    int linecount,
                    ID eventid,
                    ID variableid,
                    std::vector<int> stateID,
                    std::vector<int> stateCountMultiplier,
                    bool hasGroup,
                    int optionMulti = -1,
                    int animMulti   = -1);
    void AnimDataLineProcess(VecStr originallines,
                             VecStr& newlines,
                             std::string format,
                             std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                             std::vector<int> ASD = {0});

    void AnimDataLineProcess(AnimTemplate* originaltemplate,
                             std::shared_ptr<VecStr> newlines,
                             std::string format,
                             std::string project,
                             std::string header,
                             std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                             std::vector<int> ASD = {0});

public:
    NewAnimation(std::shared_ptr<std::unordered_map<std::string, AnimTemplate>> animlines,
                 std::string formatname,
                 std::unordered_map<std::string, VecStr> furnituretemplate,
                 int furniturecount,
                 std::string filepath,
                 AnimationInfo& behaviorOption);

    std::string GetFilePath();
    std::string GetFormatName();
    VecStr GetEventID();
    VecStr GetVariableID();
    SSMap GetMixOpt();
    ImportContainer GetAddition();
    std::vector<std::shared_ptr<AnimationInfo>> GetGroupAnimInfo();
    std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> GetGroupAddition();

    inline void newID();

    void OutputCheck(std::shared_ptr<VecStr> generatedlines,
                     proc& process,
                     condset* curset,
                     bool& norElement,
                     int& openRange,
                     size_t& elementLine,
                     int& counter,
                     ID& eventid,
                     ID& variableid,
                     std::vector<int> fixedStateID,
                     std::vector<int> stateCountMultiplier,
                     bool hasGroup,
                     bool& negative,
                     std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                     std::shared_ptr<group> groupFunction,
                     int optionMulti = -1,
                     int animMulti   = -1);
    void GetNewAnimationLine(std::shared_ptr<VecStr> generatedlines,
                             std::string behaviorFile,
                             int id,
                             ImportContainer& import,
                             ID eventid,
                             ID variableid,
                             std::vector<int>& stateID,
                             std::vector<int> stateCountMultiplier,
                             bool hasGroup,
                             bool isCore,
                             std::shared_ptr<group> groupFunction,
                             std::shared_ptr<single> singleFunction,
                             NewAnimLock& animLock);
    void GetAnimSetData(
        std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>>& newASDLines);
    void GetAnimData(std::unordered_map<std::string, std::map<std::string, VecStr>>& newAnimDataLines);
    void existingASDProcess(VecStr ASDLines, std::map<int, VecStr>& extract, std::vector<int> ASD = {0});
    void hasProcessing(std::string& line,
                       bool& norElement,
                       int& openRange,
                       int i,
                       std::shared_ptr<VecStr> generatedlines,
                       size_t& elementLine,
                       int& counter,
                       ID& eventid,
                       ID& variableid,
                       std::vector<int> fixedStateID,
                       std::vector<int> stateCountMultiplier,
                       bool hasGroup,
                       bool& negative,
                       std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                       std::shared_ptr<group> groupFunction,
                       int optionMulti = -1,
                       int animMulti   = -1);
    void multiChoice(std::string& line,
                     VecStr& storeline,
                     std::vector<std::unordered_map<std::string, bool>> groupOptionPicked,
                     int numline,
                     AnimationUtility utility);

    //void addAnimData(std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> animdata);
    void addAnimData(std::unordered_map<std::string, std::unordered_map<std::string, AnimTemplate>> animdata);

    //void addAnimSetData(std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>> animsetdata);
    void addAnimSetData(
        std::unordered_map<std::string, std::map<std::string, AnimTemplate, alphanum_less>> animsetdata);

    void storeAnimObject(VecStr animobjects, std::string listFilename, int lineCount);
    void addGroupAnimInfo(std::vector<std::shared_ptr<AnimationInfo>> animInfo);
    void setZeroEvent(std::string eventname);
    void setZeroVariable(std::string variablename);
    void setLastOrder(int order);
    void setOrder(int curOrder);

    int getOrder();
    int getNextID(std::string behavior);

    bool isLast();
    bool isKnown();
};

inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened)
{
    for (int k = condition - 1; k > 0; k--)
    {
        if (!IsConditionOpened[k]) { return false; }
    }

    return true;
}

template <class T>
int numDigits(T number);
bool clearGroupNum(std::string option2,
                   std::unordered_map<std::string, bool> optionPicked,
                   bool isNot,
                   std::unordered_map<std::string, bool> groupOption);
void motionDataReplacer(std::string& change,
                        std::string format,
                        std::string filename,
                        std::string behaviorFile,
                        int linecount,
                        std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo,
                        int order,
                        int lastOrder,
                        bool isLastOrder,
                        int animMulti,
                        bool& isChange);
void rotationDataReplacer(std::string& change,
                          std::string format,
                          std::string filename,
                          std::string behaviorFile,
                          int linecount,
                          std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo,
                          int order,
                          int lastOrder,
                          bool isLastOrder,
                          int animMulti,
                          bool& isChange);
bool isEquation(std::string& line);
int openEndBracket(std::string& line,
                   char openBrac,
                   char closeBrac,
                   std::string format,
                   std::string filename,
                   int linecount);
void CRC32Replacer(std::string& line, std::string format, std::string behaviorFile, int linecount);
void addOnReplacer(std::string& line,
                   std::string filename,
                   std::unordered_map<std::string, VecStr> addOn,
                   ImportContainer addition,
                   std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> groupAddition,
                   SSMap mixOpt,
                   uint count,
                   std::string format,
                   int numline);
void animObjectReplacer(std::string& line,
                        std::string filename,
                        std::unordered_map<int, VecStr> AnimObjec,
                        std::string format,
                        int linecount,
                        int optionMulti,
                        bool otherAnim      = false,
                        std::string animNum = "");
void eventIDReplacer(std::string& line,
                     std::string format,
                     std::string filename,
                     ID eventid,
                     std::string firstEvent,
                     int linecount);
void variableIDReplacer(std::string& line,
                        std::string format,
                        std::string filename,
                        ID variableid,
                        std::string firstVariable,
                        int linecount);
VecStr GetOptionInfo(std::string line,
                     std::string format,
                     std::string filename,
                     int numline,
                     size_t lastOrder,
                     std::vector<std::shared_ptr<AnimationInfo>> groupAnimInfo,
                     bool allowNoFixAnim,
                     bool isCondition,
                     int order       = -1,
                     int animMulti   = -1,
                     int optionMulti = -1);

#endif

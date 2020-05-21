#ifndef GROUPTEMPLATE_H_
#define GROUPTEMPLATE_H_

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "generate/animation/newanimation.h"

#include "utilities/regex.h"

typedef std::set<std::string> SetStr;
typedef std::vector<std::string> VecStr;
typedef std::unordered_set<std::string> USetStr;
typedef std::unordered_map<std::string, int> ID;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct master;

struct JointTemplate
{
    ID eventid;
    ID variableid;

    std::string zeroEvent;
    std::string zeroVariable;

    virtual void processing(std::string& line,
                            std::string filename,
                            std::string masterFormat,
                            int linecount,
                            ID eventid,
                            ID variableid,
                            int groupMulti          = -1,
                            int optionMulti         = -1,
                            int animMulti           = -1,
                            std::string multiOption = "")
    {}

    virtual void processing(std::string& line,
                            std::string filename,
                            int curFunctionID,
                            int linecount,
                            ID eventid,
                            ID variableid,
                            int groupMulti,
                            int optionMulti         = -1,
                            int animMulti           = -1,
                            std::string multiOption = "")
    {}
};

struct GroupTemplate : public JointTemplate
{
private:
    VecStr templatelines;
    NewAnimLock* atomicLock;
    ImportContainer* newImport;
    std::string filename, format;
    int* nextFunctionID;
    SSMap IDExist;
    std::shared_ptr<master> subFunctionIDs;
    std::shared_ptr<AnimTemplate> grouptemplate;
    proc* process;

    void OutputGroupBackup(std::shared_ptr<VecStr> functionline,
                           std::string format,
                           std::string masterFormat,
                           std::string behaviorFile,
                           int groupCount,
                           VecStr templatelines,
                           std::unordered_map<int, bool>& IsConditionOpened,
                           std::vector<std::vector<std::unordered_map<std::string, bool>>> masterOptionPicked,
                           std::vector<int> fixedStateID);

public:
    std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo;

    GroupTemplate(VecStr groupfunctionformat, std::shared_ptr<AnimTemplate> _grouptemplate);
    void getFunctionLines(std::shared_ptr<VecStr> functionline,
                          std::string behaviorFile,
                          std::string formatname,
                          std::vector<int>& stateID,
                          std::shared_ptr<master> newSubFunctionIDs,
                          std::vector<std::vector<std::shared_ptr<AnimationInfo>>> newGroupAnimInfo,
                          int* nFunctionID,
                          ImportContainer& import,
                          ID eventid,
                          ID variableID,
                          std::string masterFormat,
                          NewAnimLock& atomicLock,
                          int groupCount = 0);
    void stateReplacer(std::string& line,
                       std::string filename,
                       std::string statenum,
                       int ID,
                       int stateID,
                       int linecount,
                       int groupMulti);
    void processing(std::string& line,
                    std::string filename,
                    std::string masterFormat,
                    int linecount,
                    ID eventid,
                    ID variableid,
                    int groupMulti          = -1,
                    int optionMulti         = -1,
                    int animMulti           = -1,
                    std::string multiOption = "");
    std::string newID();
    void setZeroEvent(std::string eventname);
    void setZeroVariable(std::string variablename);
};

struct ExistingFunction : public JointTemplate
{
private:
    bool m_hasGroup;
    int* nextFunctionID;
    ImportContainer* newImport;
    std::string format;
    SSMap IDExist;
    std::shared_ptr<master> subFunctionIDs;
    std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo;
    std::atomic_flag* nodeIDLock;

public:
    VecStr
    groupExistingFunctionProcess(int curFunctionID,
                                 VecStr existingFunctionLines,
                                 std::shared_ptr<master> newSubFunctionIDs,
                                 std::vector<std::vector<std::shared_ptr<AnimationInfo>>> newGroupAnimInfo,
                                 std::string format,
                                 ImportContainer& import,
                                 ID newEventID,
                                 ID newVariableID,
                                 int& nFunctionID,
                                 bool hasMaster,
                                 bool hasGroup,
                                 SetStr templateGroup,
                                 bool ignoreGroup,
                                 std::atomic_flag& nodeIDFlag);
    void processing(std::string& line,
                    std::string filename,
                    int curFunctionID,
                    int linecount,
                    ID eventid,
                    ID variableid,
                    int groupMulti,
                    int optionMulti         = -1,
                    int animMulti           = -1,
                    std::string multiOption = "");
    std::string newID();
    void setZeroEvent(std::string eventname);
    void setZeroVariable(std::string variablename);
};

inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened);
VecStr GetOptionInfo(std::string line,
                     std::string format,
                     std::string filename,
                     int numline,
                     std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                     bool allowNoFixAnim,
                     bool isCondition,
                     int groupMulti          = -1,
                     int animMulti           = -1,
                     int optionMulti         = -1,
                     std::string multiOption = "");
void nonGroupOptionInfo(VecStr& optionInfo,
                        std::string line,
                        std::string format,
                        std::string filename,
                        int numline,
                        std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                        bool allowNoFixAnim,
                        bool isCondition,
                        int groupMulti,
                        int animMulti,
                        int optionMulti,
                        std::string multiOption);
void optionLimiter(VecStr optionInfo,
                   std::string line,
                   std::string format,
                   std::string filename,
                   int numline,
                   std::vector<std::vector<std::shared_ptr<AnimationInfo>>> groupAnimInfo,
                   int open,
                   bool isCondition,
                   int curGroup,
                   int curOrder,
                   int limiter,
                   int optionMulti);
std::string optionOrderProcess(std::string line,
                               std::string format,
                               std::string filename,
                               int numline,
                               int groupMulti,
                               int lastOrder,
                               bool isCondition,
                               bool allowNoFixAnim);

#endif

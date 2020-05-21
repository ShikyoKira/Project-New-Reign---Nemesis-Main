#ifndef ANIMATIONTHREAD_H_
#define ANIMATIONTHREAD_H_

#include <atomic>
#include <memory>

#include "generate/animation/animationinfo.h"

typedef std::unordered_map<std::string, int> ID;

class NewAnimation;

struct single;
struct group;
struct master;
struct GroupTemplate;

struct NewAnimLock
{
    std::atomic_flag coreLock   = ATOMIC_FLAG_INIT;
    std::atomic_flag subIDLock  = ATOMIC_FLAG_INIT;
    std::atomic_flag nodeIDLock  = ATOMIC_FLAG_INIT;
    std::atomic_flag exportLock = ATOMIC_FLAG_INIT;
};

struct NewAnimArgs
{
public:
    std::string modID, lowerBehaviorFile, ZeroEvent, ZeroVariable;
    int* lastID;
    bool core, ignoreGroup, hasGroup;
    bool skip = false;
    bool& isCoreDone;
    std::shared_ptr<NewAnimation> dummyAnimation;
    std::unordered_map<int, int>& functionState;
    ImportContainer& exportID;
    ID &eventid, variableid;
    NewAnimLock& atomicLock;
    std::vector<int> stateID, stateMultiplier;
    std::shared_ptr<single> singleFunctionIDs;
    std::shared_ptr<group> subFunctionIDs;
    std::shared_ptr<master> groupFunctionIDs;
    std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo;
    std::shared_ptr<VecStr> allEditLines;

    NewAnimArgs(std::string n_modID,
                std::string n_lowerBehaviorFile,
                int* n_lastID,
                bool n_core,
                std::shared_ptr<NewAnimation> n_newAnimation,
                std::shared_ptr<VecStr> n_allEditLines,
                bool& n_isCoreDone,
                std::unordered_map<int, int>& n_functionState,
                ImportContainer& n_exportID,
                ID& n_eventid,
                ID& n_variableid,
                NewAnimLock& n_atomicLock,
                std::string n_ZeroEvent,
                std::string n_ZeroVariable,
                bool n_hasGroup,
                std::vector<int> n_stateID,
                std::vector<int> n_stateMultiplier,
                std::shared_ptr<single> n_singleFunctionIDs,
                std::shared_ptr<group> n_subFunctionIDs,
                std::shared_ptr<master> n_groupFunctionIDs,
                bool n_ignoreGroup,
                std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& n_groupAnimInfo)
        : isCoreDone(n_isCoreDone)
        , functionState(n_functionState)
        , exportID(n_exportID)
        , eventid(n_eventid)
        , variableid(n_variableid)
        , atomicLock(n_atomicLock)
        , groupAnimInfo(n_groupAnimInfo)
    {
        allEditLines      = n_allEditLines;
        modID             = n_modID;
        lowerBehaviorFile = n_lowerBehaviorFile;
        core              = n_core;
        ZeroEvent         = n_ZeroEvent;
        ZeroVariable      = n_ZeroVariable;
        ignoreGroup       = n_ignoreGroup;
        hasGroup          = n_hasGroup;
        stateID           = n_stateID;
        stateMultiplier   = n_stateMultiplier;
        lastID            = n_lastID;
        singleFunctionIDs = n_singleFunctionIDs;
        subFunctionIDs    = n_subFunctionIDs;
        groupFunctionIDs  = n_groupFunctionIDs;
        dummyAnimation    = n_newAnimation;
    }
};

struct newGroupArgs
{
public:
    std::string templateCode, lowerBehaviorFile, ZeroEvent, ZeroVariable, filename;
    int* lastID, groupCount;
    std::vector<int> stateID;
    std::shared_ptr<GroupTemplate> groupTemp;
    ImportContainer& exportID;
    ID &eventid, variableid;
    NewAnimLock& atomicLock;
    std::shared_ptr<master> groupFunctionIDs;
    std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo;
    std::shared_ptr<VecStr> allEditLines;

    newGroupArgs(std::string n_templateCode,
                 std::string n_lowerBehaviorFile,
                 std::string n_filename,
                 int* n_lastID,
                 int n_groupCount,
                 std::vector<int> n_stateID,
                 std::shared_ptr<GroupTemplate> n_groupTemp,
                 std::shared_ptr<VecStr> n_allEditLines,
                 ImportContainer& n_exportID,
                 ID& n_eventid,
                 ID& n_variableid,
                 NewAnimLock& n_atomicLock,
                 std::string n_ZeroEvent,
                 std::string n_ZeroVariable,
                 std::shared_ptr<master> n_groupFunctionIDs,
                 std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& n_groupAnimInfo)
        : exportID(n_exportID)
        , eventid(n_eventid)
        , variableid(n_variableid)
        , atomicLock(n_atomicLock)
        , groupAnimInfo(n_groupAnimInfo)
    {
        templateCode      = n_templateCode;
        lowerBehaviorFile = n_lowerBehaviorFile;
        ZeroEvent         = n_ZeroEvent;
        ZeroVariable      = n_ZeroVariable;
        filename          = n_filename;
        lastID            = n_lastID;
        groupCount        = n_groupCount;
        stateID           = n_stateID;
        groupTemp         = n_groupTemp;
        groupFunctionIDs  = n_groupFunctionIDs;
        allEditLines      = n_allEditLines;
    }
};

#endif

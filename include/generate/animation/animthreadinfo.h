#pragma once

#include "Global.h"

#include "generate/animation/templateprocessing.h"

using SSMap = std::unordered_map<std::string, std::string>;
using ImportContainer = std::unordered_map<std::string, SSMap>;

class NewAnimation;

struct group;
struct NewAnimLock;
struct GroupTemplate;

struct AnimThreadInfo
{
    NewAnimLock* animLock;

    std::string* line;
    size_t numline;

    std::string filepath;
    std::string filename;
    std::string mainAnimEvent;
    std::string zeroEvent;
    std::string zeroVariable;
    std::string project;
    std::string header;

    bool hasDuration;
     
    double duration;

    int order;
    int lastorder;
    int furnitureCount;

    const std::unordered_map<int, VecStr> AnimObject;
    const std::vector<std::unordered_map<std::string, bool>> groupOptionPicked;
    const std::vector<std::vector<std::unordered_map<std::string, bool>>> masterOptionPicked;
    const std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> groupAddition;

    std::string strID;
    std::string multiOption = "";

    bool hasGroup;
    bool isMaster;
    bool& negative;
    bool& isEnd;
    bool& norElement;
    bool& elementCatch;

    int optionMulti = -1;
    int animMulti   = -1;
    int groupMulti  = 0;
    int& openRange;
    int& counter;

    size_t& elementLine;
    size_t stateCount = 0;

    ID& eventid;
    ID& variableid;

    std::vector<int>& fixedStateID;
    std::vector<int>& stateCountMultiplier;

    std::vector<choice_c> failed;
    std::shared_ptr<choice_c> captured;

    SSMap IDExist;

    const ImportContainer addition;
    ImportContainer* newImport;

    NewAnimation* curAnim;
    GroupTemplate* curGroup;

    std::shared_ptr<group> groupFunction;
    std::shared_ptr<master> masterFunction;

    std::shared_ptr<VecStr> generatedlines;

    AnimThreadInfo(const std::string& _filepath,
                   const std::string& _filename,
                   const std::string& _mainAnimEvent,
                   const std::string& _zeroEvent,
                   const std::string& _zeroVariable,
                   bool _hasGroup,
                   bool& _negative,
                   bool& _isEnd,
                   bool& _norElement,
                   bool& _elementCatch,
                   bool hasDuration,
                   double duration,
                   int& _openRange,
                   int& _counter,
                   size_t& _elementLine,
                   int _furnitureCount,
                   ID& _eventid,
                   ID& _variableid,
                   std::vector<int>& _fixedStateID,
                   std::vector<int>& _stateCountMultiplier,
                   int _order,
                   int _lastorder,
                   const SSMap& _IDExist,
                   const std::unordered_map<int, VecStr>& _AnimObject,
                   const ImportContainer& _addition,
                   ImportContainer* _newImport,
                   const std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>& _groupAddition,
                   const std::vector<std::unordered_map<std::string, bool>>& _groupOptionPicked,
                   const std::vector<std::vector<std::unordered_map<std::string, bool>>>& _masterOptionPicked,
                   std::shared_ptr<group> _groupFunction,
                   std::shared_ptr<VecStr> _generatedlines,
                   NewAnimation* _curAnim,
                   NewAnimLock* _animLock);
};

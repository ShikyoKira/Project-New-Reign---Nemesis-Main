#include "utilities/conditions.h"

#include "generate/animation/animthreadinfo.h"

using namespace std;

AnimThreadInfo::AnimThreadInfo(
    const std::string& _filepath,
    const std::string& _filename,
    const std::string& _mainAnimEvent,
    const std::string& _zeroEvent,
    const std::string& _zeroVariable,
    bool _hasGroup,
    bool& _negative,
    bool& _isEnd,
    bool& _norElement,
    bool& _elementCatch,
    bool _hasDuration,
    double _duration,
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
    NewAnimLock* _animLock)
    : filepath(_filepath)
    , filename(_filename)
    , mainAnimEvent(_mainAnimEvent)
    , zeroEvent(_zeroEvent)
    , zeroVariable(_zeroVariable)
    , hasGroup(_hasGroup)
    , negative(_negative)
    , isEnd(_isEnd)
    , norElement(_norElement)
    , elementCatch(_elementCatch)
    , hasDuration(_hasDuration)
    , duration(_duration)
    , openRange(_openRange)
    , counter(_counter)
    , elementLine(_elementLine)
    , furnitureCount(_furnitureCount)
    , eventid(_eventid)
    , variableid(_variableid)
    , fixedStateID(_fixedStateID)
    , stateCountMultiplier(_stateCountMultiplier)
    , order(_order)
    , lastorder(_lastorder)
    , IDExist(_IDExist)
    , AnimObject(_AnimObject)
    , addition(_addition)
    , newImport(_newImport)
    , groupAddition(_groupAddition)
    , groupOptionPicked(_groupOptionPicked)
    , masterOptionPicked(_masterOptionPicked)
    , groupFunction(_groupFunction)
{
    generatedlines = _generatedlines;
    curAnim        = _curAnim;
    animLock       = _animLock;
}

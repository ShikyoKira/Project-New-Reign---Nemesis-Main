#ifndef NODEJOINT_H_
#define NODEJOINT_H_

#include "Global.h"
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using ID              = std::unordered_map<std::string, int>;
using VecStr          = std::vector<std::string>;
using SetStr          = std::set<std::string>;
using UMapStr2           = std::unordered_map<std::string, std::string>;
using ImportContainer = std::unordered_map<std::string, UMapStr2>;

struct master;
struct NodeJoint;
struct JointTemplate;
struct AnimationInfo;

struct LineCheck
{
    bool deleted = true;
    bool process = false;
    size_t row;
    std::string line;
    std::shared_ptr<NodeJoint> nested;

    LineCheck(std::string _line, size_t _row);
    LineCheck(std::shared_ptr<NodeJoint> _nested, size_t _row);

    LineCheck& operator=(std::string _line);
    LineCheck& operator=(std::shared_ptr<NodeJoint> _nested);
};

struct NodePackedParameters
{
    std::string format;
    std::string filename;
    std::string behaviorFile;
    int& nextFunctionID;
    std::string& strID;
    const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo;
    UMapStr2& IDExist;
    std::shared_ptr<master> subFunctionIDs;
    ImportContainer& import;
    int linecount;
    ID eventid;
    ID variableid;
    std::string zeroEvent;
    std::string zeroVariable;
    int groupMulti;
    int animMulti;
    int optionMulti;
    bool hasGroup;
    std::string multiOption;
    std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked;
    unsigned __int64& openRange;
    size_t& elementCount;
    std::string& line;
    LineCheck* elementCatch;
    bool& negative;

    NodePackedParameters(std::string _format,
                         std::string _filename,
                         std::string _behaviorFile,
                         int& _nextFunctionID,
                         std::string& _strID,
                         const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& _groupAnimInfo,
                         UMapStr2& _IDExist,
                         std::shared_ptr<master> _subFunctionIDs,
                         ImportContainer& _import,
                         int _linecount,
                         ID _eventid,
                         ID _variableid,
                         std::string _zeroEvent,
                         std::string _zeroVariable,
                         int _groupMulti,
                         int _animMulti,
                         int _optionMulti,
                         bool _hasGroup,
                         std::string _multiOption,
                         std::vector<std::vector<std::unordered_map<std::string, bool>>>& _optionPicked,
                         unsigned __int64& _openRange,
                         size_t& _elementCount,
                         std::string& _line,
                         LineCheck* _elementCatch,
                         bool& _negative);
};

struct NodeJoint
{
    enum FuncType
    {
        NONE,
        NEW,
        FOREACH,
        CONDITION_START,
        CONDITION_ELSE,
        CONDITION
    };

    bool conditionSkip = false;
    size_t row;
    FuncType type     = NONE;
    std::string condt = "";
    std::vector<LineCheck> storeTemplate;
    std::vector<std::vector<LineCheck>> output;
    std::unordered_map<std::string, std::vector<size_t>>
        templateSection; // template code, list of index of template section
    std::string behaviorFile;
    SetStr templateGroup;

    NodeJoint(VecStr& node,
              std::string format,
              std::string filename,
              std::string _behaviorFile,
              std::unordered_map<std::string, std::string> otherAnimType,
              size_t startline = 0,
              size_t size      = -1);
    NodeJoint(VecStr& node,
              std::string format,
              std::string filename,
              std::string _behaviorFile,
              SetStr _templateGroup,
              size_t startline = 0,
              size_t size      = -1);

    void insertData(std::string format,
                    std::string filename,
                    std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked,
                    const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo,
                    int groupMulti,
                    int animMulti,
                    int optionMulti,
                    bool hasMaster,
                    bool hasGroup,
                    bool ignoreGroup,
                    std::string multiOption,
                    int& nextFunctionID,
                    std::string& strID,
                    UMapStr2& IDExist,
                    ImportContainer& import,
                    ID eventid,
                    ID variableid,
                    std::string zeroEvent,
                    std::string zeroVariable,
                    unsigned __int64& openRange,
                    size_t elementCount,
                    LineCheck* elementCatch,
                    std::shared_ptr<master> subFunctionIDs,
                    bool& negative);

    void forEachProcess(std::vector<std::vector<LineCheck>>& output,
                        LineCheck& storeTemplate,
                        std::string condition,
                        size_t sect,
                        std::string format,
                        std::string filename,
                        std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked,
                        const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo,
                        int groupMulti,
                        int animMulti,
                        int optionMulti,
                        bool hasMaster,
                        bool hasGroup,
                        bool ignoreGroup,
                        std::string multiOption,
                        int& nextFunctionID,
                        std::string& strID,
                        UMapStr2& IDExist,
                        ImportContainer& import,
                        ID eventid,
                        ID variableid,
                        std::string zeroEvent,
                        std::string zeroVariable,
                        unsigned __int64 openRange,
                        size_t elementCount,
                        LineCheck* elementCatch,
                        std::shared_ptr<master> subFunctionIDs,
                        bool& negative);
    static void optionMultiLoop(std::vector<LineCheck>& output,
                                std::string format,
                                std::string filename,
                                std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked,
                                const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo,
                                int groupMulti,
                                int animMulti,
                                int optionMulti,
                                bool ignoreGroup,
                                std::string multiOption,
                                int& nextFunctionID,
                                std::string& strID,
                                UMapStr2& IDExist,
                                ImportContainer& import,
                                ID eventid,
                                ID variableid,
                                std::string zeroEvent,
                                std::string zeroVariable,
                                unsigned __int64 openRange,
                                size_t elementCount,
                                LineCheck* elementCatch,
                                std::shared_ptr<master> subFunctionIDs,
                                bool& negative,
                                LineCheck temp);

    VecStr unpack(); // output all stored outputs in different layers

private:
    void dataBake(VecStr& node,
                  std::string format,
                  std::string filename,
                  std::unordered_map<std::string, std::string> otherAnimType,
                  size_t startline = 0,
                  size_t size      = -1);
};

#endif

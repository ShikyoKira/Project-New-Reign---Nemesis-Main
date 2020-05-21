#ifndef BEHAVIORPROCESS_UTILITY_H_
#define BEHAVIORPROCESS_UTILITY_H_

#include <map>
#include <memory>

struct master;
struct AnimationInfo;

typedef std::set<std::string> SetStr;
typedef std::unordered_map<std::string, int> ID;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct NewAnimArgs;
struct newGroupArgs;
struct NodeJoint;

void animThreadStart(std::shared_ptr<NewAnimArgs> args);
void groupThreadStart(std::shared_ptr<newGroupArgs> args);
void elementUpdate(size_t& elementLine, int& counter, int& curID, std::map<int, VecStr>& catalystMap);
void unpackToCatalyst(std::map<int, VecStr>& catalystMap,
                      std::unordered_map<int, std::shared_ptr<NodeJoint>>& existingNodes);

void processExistFuncID(std::vector<int>& funcIDs,
                        const std::string& ZeroEvent,
                        const std::string& ZeroVariable,
                        std::map<int, VecStr>& catalystMap,
                        const std::shared_ptr<master>& groupFunctionIDs,
                        const std::vector<std::vector<std::shared_ptr<AnimationInfo>>>& groupAnimInfo,
                        const std::string& templateCode,
                        ImportContainer& exportID,
                        ID& eventid,
                        ID& variableid,
                        int& lastID,
                        bool hasMaster,
                        bool hasGroup,
                        SetStr& templateGroup,
                        bool ignoreGroup,
                        const std::string& behaviorFile,
                        std::unordered_map<int, std::shared_ptr<NodeJoint>>& existingNodes);

#endif

#ifndef BEHAVIORPROCESS_UTILITY_H_
#define BEHAVIORPROCESS_UTILITY_H_

#include <memory>

#include "Global.h"

struct master;
struct animationInfo;

typedef std::set<std::string> setstr;
typedef std::unordered_map<std::string, int> id;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct newAnimArgs;
struct newGroupArgs;
struct nodeJoint;

void animThreadStart(std::shared_ptr<newAnimArgs> args);
void groupThreadStart(std::shared_ptr<newGroupArgs> args);
void elementUpdate(size_t& elementLine, int& counter, int& curID, std::map<int, vecstr>& catalystMap);
void unpackToCatalyst(std::map<int, vecstr>& catalystMap, std::unordered_map<int, std::shared_ptr<nodeJoint>>& existingNodes);
void processExistFuncID(std::vector<int>& funcIDs, std::string ZeroEvent, std::string ZeroVariable, std::map<int, vecstr>& catalystMap, std::shared_ptr<master> groupFunctionIDs,
	std::vector<std::vector<std::shared_ptr<animationInfo>>>& groupAnimInfo, std::string templateCode, ImportContainer& exportID, id& eventid, id& variableid, int lastID,
	bool hasMaster, bool hasGroup, setstr& templateGroup, bool ignoreGroup, std::unordered_map<int, std::shared_ptr<nodeJoint>> existingNodes);

#endif
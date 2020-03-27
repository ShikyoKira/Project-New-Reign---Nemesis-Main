#ifndef BEHAVIORPROCESS_UTILITY_H_
#define BEHAVIORPROCESS_UTILITY_H_

#include <Global.h>
#include <memory>

struct master;
struct AnimationInfo;

typedef std::set<std::string> setstr;
typedef std::unordered_map<std::string, int> id;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct NewAnimArgs;
struct newGroupArgs;
struct NodeJoint;

namespace boost
{
	namespace asio
	{
		class thread_pool;
	}
}

void animThreadStart(std::shared_ptr<NewAnimArgs> args, boost::asio::thread_pool* mt);
void groupThreadStart(std::shared_ptr<newGroupArgs> args, boost::asio::thread_pool* mt);
void elementUpdate(size_t& elementLine, int& counter, int& curID, std::map<int, vecstr>& catalystMap);
void unpackToCatalyst(std::map<int, vecstr>& catalystMap, std::unordered_map<int, std::shared_ptr<NodeJoint>>& existingNodes);
void processExistFuncID(std::vector<int> &funcIDs,
                        std::string ZeroEvent,
                        std::string ZeroVariable,
                        std::map<int, vecstr> &catalystMap,
                        std::shared_ptr<master> groupFunctionIDs,
                        const std::vector<std::vector<std::shared_ptr<AnimationInfo>>> &groupAnimInfo,
                        std::string templateCode,
                        ImportContainer &exportID,
                        id &eventid,
                        id &variableid,
                        int &lastID,
                        bool hasMaster,
                        bool hasGroup,
                        setstr &templateGroup,
                        bool ignoreGroup,
                        std::string behaviorFile,
                        std::unordered_map<int, std::shared_ptr<NodeJoint>> &existingNodes);

#endif

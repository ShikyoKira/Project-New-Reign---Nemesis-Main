#include "scope/stateidmanager.h"

#include "utilities/animqueryfile.h"

#include "hkx/HkxBehaviorFile.h"

size_t nemesis::StateIdManager::CreateStateId(const nemesis::AnimQuery& query,
                                            size_t index,
                                            const std::string& node_id)
{
    return CreateStateId(query, index, node_id, id_byquery);
}

size_t nemesis::StateIdManager::CreateStateId(const nemesis::AnimQueryFile& querylist,
                                            size_t index,
                                            const std::string& node_id)
{
    return CreateStateId(querylist, index, node_id, id_bygroup);
}

size_t nemesis::StateIdManager::CreateStateId(const nemesis::Template& tmplt,
                                            size_t index,
                                            const std::string& node_id)
{
    return CreateStateId(tmplt, index, node_id, id_bytemplate);
}

int nemesis::StateIdManager::StateIdFrom(const nemesis::AnimQuery& query, size_t index)
{
    return StateIdFrom(query, index, id_byquery);
}

int nemesis::StateIdManager::StateIdFrom(const nemesis::AnimQueryFile& querylist, size_t index)
{
    return StateIdFrom(querylist, index, id_bygroup);
}

int nemesis::StateIdManager::StateIdFrom(const nemesis::Template& tmplt, size_t index)
{
    return StateIdFrom(tmplt, index, id_bytemplate);
}

nemesis::StateIdManager::StateIdManager(const HkxBehaviorFile& behavior)
    : behavior(behavior)
{
    for (auto& statelink : behavior.statelinks)
    {
        size_t id          = behavior.topstate.at(statelink.first);
        SPtr<size_t> idptr = std::make_shared<size_t>(id);

        for (auto& temp_nodeid : statelink.second)
        {
            nextids[temp_nodeid].emplace_back(idptr);
        }
    }
}

size_t nemesis::StateIdManager::GetStateId(const nemesis::AnimQuery& query,
                                         size_t index,
                                         const std::string& node_id)
{
    return Template_GetStateId(query, index, node_id);
}

size_t nemesis::StateIdManager::GetStateId(const nemesis::AnimQueryFile& querylist,
                                         size_t index,
                                         const std::string& node_id)
{
    return Template_GetStateId(querylist, index, node_id);
}

size_t nemesis::StateIdManager::GetStateId(const nemesis::Template& tmplt,
                                         size_t index,
                                         const std::string& node_id)
{
    return Template_GetStateId(tmplt, index, node_id);
}

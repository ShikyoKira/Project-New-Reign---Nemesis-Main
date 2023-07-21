#pragma once

#include "utilities/types.h"

namespace nemesis
{
    struct AnimQuery;
    struct AnimQueryFile;
    struct Template;
    struct HkxBehaviorFile;

    struct StateIdManager
    {
    private:
        const HkxBehaviorFile& behavior;

        UMap<const nemesis::AnimQuery*, UMap<size_t, size_t>> id_byquery;
        UMap<const nemesis::AnimQueryFile*, UMap<size_t, size_t>> id_bygroup;
        UMap<const nemesis::Template*, UMap<size_t, size_t>> id_bytemplate;

        // template node id, list of highest state id. Reasoning in #Ref 1.0
        UMap<std::string, Vec<SPtr<size_t>>> nextids;

        template<typename T>
        size_t CreateStateId(T& key, size_t index, const std::string& node_id, UMap<T*, UMap<size_t, size_t>> id_map)
        {
            auto itr = nextids.find(node_id);

            if (itr == nextids.end())
            {
                id_map[&key][index] = 0;
                nextids[node_id].emplace_back(std::make_shared<size_t>(1));
                return 0;
            }

            size_t highest = 0;

            for (auto& topid : itr->second)
            {
                if (*topid <= highest) continue;

                highest = *topid;
            }

            id_map[&key][index] = highest;
            highest++;

            for (auto& topid : itr->second)
            {
                *topid = highest;
            }

            return highest - 1;
        }

        size_t CreateStateId(const nemesis::AnimQuery& query, size_t index, const std::string& node_id);
        size_t CreateStateId(const nemesis::AnimQueryFile& querylist, size_t index, const std::string& node_id);
        size_t CreateStateId(const nemesis::Template& tmplt, size_t index, const std::string& node_id);

        template <typename T>
        int StateIdFrom(T& key, size_t index, UMap<T*, UMap<size_t, size_t>> id_map)
        {
            auto itr = id_map.find(&key);

            if (itr != id_map.end())
            {
                auto index_itr = itr->second.find(index);

                if (index_itr != itr->second.end()) return index_itr->second;
            }

            return -1;
        }

        int StateIdFrom(const nemesis::AnimQuery& query, size_t index);
        int StateIdFrom(const nemesis::AnimQueryFile& querylist, size_t index);
        int StateIdFrom(const nemesis::Template& tmplt, size_t index);

        template <typename T>
        size_t Template_GetStateId(T& key, size_t index, const std::string& node_id)
        {
            int id = StateIdFrom(key, index);

            if (id >= 0) return id;

            return CreateStateId(key, index, node_id);
        }

    public:
        StateIdManager(const HkxBehaviorFile& behavior);

        size_t GetStateId(const nemesis::AnimQuery& query, size_t index, const std::string& node_id);
        size_t GetStateId(const nemesis::AnimQueryFile& querylist, size_t index, const std::string& node_id);
        size_t GetStateId(const nemesis::Template& tmplt, size_t index, const std::string& node_id);
    };
}


/*
#Ref 1.0

There is a possibility of a single template state machine info being the child multiple state machine

--------------------
ILLUSTRATION
--------------------

syntax
SM1 = State machine 1
SM2 = State machine 2

TSMI1 = Template state machine info 1
TSMI2 = Template state machine info 2


SM1, max id = 10
SM1 has TSMI1

SM2, max id = 2
SM2 has TSMI1 and TSMI2


TSMI1 expected new id is 11
TSMI2 expected new id is 3

If SM2's max id goes up to 20, but SM1's max id is still 10...
TSMI1 expected new id is 21
TSMI2 expected new id is 21

--------------------
END ILLUSTRATION
--------------------

Above illustration shows that TSMI1 has to take the highest id from SM1 and SM2
Meanwhile, TSMI2 only need to focus on the hightst id in SM2

This illustration only shows 2 state machines, its entirely possible to have more than 2 state machines

*/


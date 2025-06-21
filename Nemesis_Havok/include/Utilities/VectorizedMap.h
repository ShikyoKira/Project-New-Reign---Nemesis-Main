#pragma once

#include "Nemesis_Havok/include/Utilities/Types.h"

namespace nemesis
{
    template <typename Key, typename Value>
    struct VectorizedMap
    {
    private:
        using KeyValuePair = Pair<Key, Value>;
        using ListIterator = typename Deq<Key>::iterator;

        UMap<Key, Pair<Value, ListIterator>> KVMap;
        Deq<Key> Order;

    public:
        ~VectorizedMap()
        {
            KVMap.clear();
            Order.clear();
        }

        void emplace_back(const Key& key, const Value& value)
        {
            auto it = KVMap.find(key);

            if (it != KVMap.end())
            {
                it->second.first = value;
                return;
            }

            Order.emplace_back(key);
            KVMap[key] = {value, std::prev(Order.end())};
        }

        void emplace_front(const Key& key, const Value& value)
        {
            auto it = KVMap.find(key);

            if (it != KVMap.end())
            {
                it->second.first = value;
                return;
            }

            Order.emplace_front(key);
            KVMap[key] = {value, std::prev(Order.end())};
        }

        Value& operator[](const Key& key)
        {
            return KVMap[key].first;
        }

        Value& at(const Key& key)
        {
            return KVMap[key].first;
        }

        const Value& at(const Key& key) const
        {
            return KVMap.at(key).first;
        }

        bool find(const Key& key) const
        {
            return KVMap.find(key) != KVMap.end();
        }

        bool try_at(const Key& key, Value& val) const
        {
            auto itr = KVMap.find(key);
            
            if (itr == KVMap.end()) return false;

            val = itr->second.first;
            return true;
        }

        bool erase(const Key& key)
        {
            auto it = KVMap.find(key);

            if (it == KVMap.end()) return false;

            Order.erase(it->second.second);
            KVMap.erase(it);
            return true;
        }

        size_t size() const
        {
            return KVMap.size();
        }

        void move_to_front(const Key& key)
        {
            for (auto itr = Order.begin(); itr != Order.end(); itr++)
            {
                if (*itr != key) continue;

                Order.erase(itr);
                Order.emplace_front(key);
                return;
            }
        }

        bool try_move_to_front(const Key& key)
        {
            for (auto itr = Order.begin(); itr != Order.end(); itr++)
            {
                if (*itr != key) continue;

                Order.erase(itr);
                Order.emplace_front(key);
                return true;
            }

            return false;
        }

        void clear()
        {
            KVMap.clear();
            Order.clear();
        }

        bool empty()
        {
            return KVMap.empty();
        }

        template <typename Func>
        void for_each(Func func) const
        {
            for (const auto& key : Order)
            {
                const auto& [value, _] = KVMap.at(key);
                func(key, value);
            }
        }
    };
}
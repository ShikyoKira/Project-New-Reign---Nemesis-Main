#pragma once

#include "Utilities/Types.h"

namespace nemesis
{
    template<typename T>
    struct Trie
    {
    private:
        struct TrieNode
        {
            UMap<char, UPtr<TrieNode>> Children;
            UPtr<T> Value;
        };

        UPtr<TrieNode> Root;

        TrieNode* TraverseOrCreate(const std::string& key)
        {
            TrieNode* current = Root.get();

            for (char ch : key)
            {
                current
                    = (current->Children[ch] ? current->Children[ch].get()
                                             : (current->Children[ch] = std::make_unique<TrieNode>()).get());
            }

            return current;
        }

    public:
        Trie()
        {
            Root = std::make_unique<TrieNode>();
        }

        void Add(const std::string& key, T&& val)
        {
            TrieNode* current = TraverseOrCreate(key);
            current->Value    = std::make_unique<T>(std::forward<T>(val));
        }

        template<typename ...Args>
        void Add(const std::string& key, Args&&... args)
        {
            TrieNode* current = TraverseOrCreate(key);
            current->Value    = std::make_unique<T>(std::forward<Args>(args)...);
        }

        const T* FindMatch(const std::string& key) const
        {
            TrieNode* current   = Root.get();
            const T* best_match = nullptr;

            for (auto& ch : key)
            {
                auto itr = current->Children.find(ch);

                if (itr == current->Children.end()) break;

                current = itr->second.get();

                if (!current->Value) continue;

                best_match = current->Value.get();
            }

            return best_match;
        }

        Deq<const T*> FindMatches(const std::string& key) const
        {
            TrieNode* current   = Root.get();
            Deq<const T*> best_matches;

            for (auto& ch : key)
            {
                auto itr = current->Children.find(ch);

                if (itr == current->Children.end()) break;

                current = itr->second.get();

                if (!current->Value) continue;

                best_matches.emplace_front(current->Value.get());
            }

            return best_matches;
        }
    };
}

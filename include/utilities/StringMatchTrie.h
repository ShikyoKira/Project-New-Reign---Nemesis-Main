#pragma once

#include <functional>

#include "utilities/types.h"

namespace nemesis
{
    template<typename ReturnType>
    struct StringMatchTrie
    {
    private:
        struct TrieNode
        {
            bool IsEnd      = false;
            bool HasCapture = false;

            TrieNode* Previous;
            std::function<ReturnType(const std::string&)> OnMatch;

            UMap<char, UPtr<TrieNode>> Children;
        };

        UPtr<TrieNode> Root;
        std::function<ReturnType()> OnNoMatch;

    public:
        StringMatchTrie(std::function<ReturnType()> callback_on_no_match) noexcept
        {
            OnNoMatch = callback_on_no_match;
            Root      = std::make_unique<TrieNode>();
        }

        void Insert(const std::string& text, std::function<ReturnType(const std::string&)> callback_on_match)
        {
            TrieNode* current = Root.get();

            for (auto& ch : text)
            {
                auto itr = current->Children.find(ch);

                if (current->Children.find(ch) != current->Children.end())
                {
                    current = itr->second.get();
                    continue;
                }

                current = (current->Children[ch] = std::make_unique<TrieNode>()).get();
            }

            current->IsEnd   = true;
            current->OnMatch = callback_on_match;
        }

        void Insert(const std::string& prefix,
                    const std::string& suffix,
                    std::function<ReturnType(const std::string&)> callback_on_match)
        {
            TrieNode* current = Root.get();

            for (auto& ch : prefix)
            {
                auto itr = current->Children.find(ch);

                if (current->Children.find(ch) != current->Children.end())
                {
                    current = itr->second.get();
                    continue;
                }

                current = (current->Children[ch] = std::make_unique<TrieNode>()).get();
            }

            current->HasCapture = true;
            TrieNode* previous  = current;

            for (auto& ch : suffix)
            {
                auto itr = current->Children.find(ch);

                if (current->Children.find(ch) != current->Children.end())
                {
                    current = itr->second.get();
                    continue;
                }

                current           = (current->Children[ch] = std::make_unique<TrieNode>()).get();
                current->Previous = previous;
            }

            current->IsEnd   = true;
            current->OnMatch = callback_on_match;
        }

        ReturnType Match(const std::string& text) const
        {
            UMap<TrieNode*, Pair<size_t, size_t>> capture;
            Deq<TrieNode*> current_list = {Root.get()};

            for (size_t k = 0; k < text.length(); k++)
            {
                auto& ch = text[k];

                for (size_t i = 0; i < current_list.size(); ++i)
                {
                    auto& current = current_list[i];
                    auto itr      = current->Children.find(ch);

                    if (itr == current->Children.end()) continue;

                    current_list.emplace_front(Root.get());
                    ++i;
                    current = itr->second.get();

                    if (current->HasCapture)
                    {
                        capture[current] = {k + 1, 0};
                        continue;
                    }
                    else if (current->IsEnd)
                    {
                        if (!current->Previous) return current->OnMatch("");

                        auto& pair = capture[current->Previous];
                        return current->OnMatch(text.substr(pair.first, pair.second));
                    }
                    else if (current->Previous)
                    {
                        auto& pair = capture[current->Previous];

                        if (pair.second == 0)
                        {
                            pair = {pair.first, k - pair.first};
                        }
                    }
                }
            }

            return OnNoMatch();
        }
    };
}

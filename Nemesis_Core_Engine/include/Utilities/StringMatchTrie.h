#pragma once

#include <functional>

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    template<typename ReturnType>
    struct StringMatchTrie
    {
    private:
        struct TrieNode
        {
            static const size_t Level1Size = 256;

            bool IsEnd      = false;
            bool HasCapture = false;

            TrieNode* Previous = nullptr;
            std::function<ReturnType(const std::string&)> OnMatch;

            UPtr<TrieNode> Children[Level1Size];

            TrieNode()
            {
                for (auto& child : Children)
                {
                    child = nullptr;
                }
            }
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
                auto& trie_node = current->Children[static_cast<unsigned char>(ch)];

                if (!trie_node)
                {
                    current = (trie_node = std::make_unique<TrieNode>()).get();
                    continue;
                }

                current = trie_node.get();
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
                auto& trie_node = current->Children[static_cast<unsigned char>(ch)];

                if (!trie_node)
                {
                    current = (trie_node = std::make_unique<TrieNode>()).get();
                    continue;
                }

                current = trie_node.get();
            }

            current->HasCapture = true;
            TrieNode* previous  = current;

            for (auto& ch : suffix)
            {
                auto& trie_node = current->Children[static_cast<unsigned char>(ch)];

                if (!trie_node)
                {
                    current = (trie_node = std::make_unique<TrieNode>()).get();
                    continue;
                }

                current = trie_node.get();
            }

            current->Previous = previous;
            current->IsEnd    = true;
            current->OnMatch  = callback_on_match;
        }

        ReturnType Match(const std::string& text) const
        {
            for (size_t k = 0; k < text.length(); k++)
            {
                auto* trie_node = Root->Children[static_cast<unsigned char>(text[k])].get();
                size_t i        = 0;
                Pair<size_t, size_t> pair;

                while (trie_node)
                {
                    if (trie_node->HasCapture)
                    {
                        pair           = {k + i + 1, 0};
                        TrieNode* node = nullptr;

                        do
                        {
                            ++i;
                            size_t next = k + i;

                            if (next == text.length()) break;

                            auto& ch = text[next];
                            node     = trie_node->Children[static_cast<unsigned char>(ch)].get();
                        } while (!node);

                        if (!node) break;

                        pair.second = k + i - pair.first;
                        trie_node = node;
                        continue;
                    }
                    else if (trie_node->IsEnd)
                    {
                        if (!trie_node->Previous) return trie_node->OnMatch("");

                        return trie_node->OnMatch(text.substr(pair.first, pair.second));
                    }

                    ++i;
                    size_t next = k + i;

                    if (next == text.length()) break;

                    auto& ch  = text[next];
                    trie_node = trie_node->Children[static_cast<unsigned char>(ch)].get();
                }
            }

            return OnNoMatch();
        }
    };
}

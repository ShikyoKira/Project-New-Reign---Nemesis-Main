#pragma once

#include "utilities/types.h"
#include "utilities/StringMatchTrie.h"

namespace nemesis
{
    struct LineStream
    {
        enum TokenType
        {
            NONE,
            IF,
            ELSE_IF,
            ELSE,
            END_IF,
            MOD_OPEN,
            MOD_ORG,
            MOD_CLOSE,
            FOR_EACH,
            CLOSE,
            BREAK
        };

        struct Token
        {
            nemesis::Line Value;
            nemesis::LineStream::TokenType Type;

            Token(const std::string& value,
                  size_t linenum,
                  const std::filesystem::path& filepath,
                  TokenType type) noexcept;
            Token(const nemesis::Line& value, TokenType type) noexcept;
        };

    private:
        struct TokenParser
        {
        private:
            StringMatchTrie<Pair<std::string, nemesis::LineStream::TokenType>> StringMatcher;

        public:
            TokenParser();

            UPtr<nemesis::LineStream::Token> GenerateToken(const nemesis::Line& line);
        };

        VecNstr::const_iterator Start;
        VecNstr::const_iterator Iterator;
        VecNstr::const_iterator End;

        UMap<int, UPtr<nemesis::LineStream::Token>> TokenPointers;

        const nemesis::LineStream::Token* GenerateToken(VecNstr::const_iterator itr);

    public:
        LineStream(VecNstr::const_iterator start, VecNstr::const_iterator end) noexcept;

        nemesis::LineStream& operator++() noexcept;
        nemesis::LineStream& operator--() noexcept;
        nemesis::LineStream& operator+=(int offset) noexcept;
        nemesis::LineStream& operator-=(int offset) noexcept;
        const nemesis::Line& operator*() noexcept;

        const nemesis::LineStream::Token& GetToken();
        const nemesis::LineStream::Token* GetForwardToken(size_t step);
        const nemesis::LineStream::Token* GetBackwardToken(size_t step);
        int GetPosition() const noexcept;
        size_t GetSize() const noexcept;

        bool IsEoF() const noexcept;
    };
}

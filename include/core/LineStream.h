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
            CLOSE
        };

        struct Token
        {
            nemesis::Line Value;
            TokenType Type;

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

    public:
        LineStream(VecNstr::const_iterator start, VecNstr::const_iterator end) noexcept;

        nemesis::LineStream& operator++() noexcept;
        nemesis::LineStream& operator--() noexcept;
        nemesis::LineStream& operator+=(int offset) noexcept;
        nemesis::LineStream& operator-=(int offset) noexcept;
        const nemesis::Line& operator*() noexcept;

        const nemesis::LineStream::Token& GetToken();
        int GetPosition() const noexcept;

        bool IsEoF() const noexcept;
    };
}
#pragma once

#include "utilities/types.h"
#include "utilities/StringMatchTrie.h"

namespace nemesis
{
    struct LineReserveStream
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
            StringMatchTrie<Pair<std::string, nemesis::LineReserveStream::TokenType>> StringMatcher;

        public:
            TokenParser();

            UPtr<nemesis::LineReserveStream::Token> GenerateToken(const nemesis::Line& line);
        };

        VecNstr::const_reverse_iterator Start;
        VecNstr::const_reverse_iterator Iterator;
        VecNstr::const_reverse_iterator End;

        UMap<int, UPtr<nemesis::LineReserveStream::Token>> TokenPointers;

        const nemesis::LineReserveStream::Token* GenerateToken(VecNstr::const_reverse_iterator itr);

    public:
        LineReserveStream(VecNstr::const_reverse_iterator start,
                          VecNstr::const_reverse_iterator end) noexcept;

        nemesis::LineReserveStream& operator++() noexcept;
        nemesis::LineReserveStream& operator--() noexcept;
        nemesis::LineReserveStream& operator+=(int offset) noexcept;
        nemesis::LineReserveStream& operator-=(int offset) noexcept;
        const nemesis::Line& operator*() noexcept;

        const nemesis::LineReserveStream::Token& GetToken();
        const nemesis::LineReserveStream::Token* GetForwardToken(size_t step);
        const nemesis::LineReserveStream::Token* GetBackwardToken(size_t step);
        int GetPosition() const noexcept;

        bool IsEoF() const noexcept;
    };
}

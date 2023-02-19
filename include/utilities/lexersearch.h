#pragma once

#include "core/animvarptr.h"

namespace nemesis
{
    struct LexerSearch
    {
    private:
        std::string keyword;
        Vec<nemesis::AnimVarPtr::Lexer> lexer_list;

        using RtnFunc = std::function<std::string(nemesis::ScopeInfo&)>;

    public:
        LexerSearch(const std::string& keyword) noexcept
            : keyword(keyword)
        {
        }

        void AddLexer(const std::string& classname)
        {
            lexer_list.emplace_back(nemesis::AnimVarPtr::Lexer(classname, keyword));
        }

        void AddLexer(const std::string& classname, RtnFunc callback)
        {
            lexer_list.emplace_back(nemesis::AnimVarPtr::Lexer(classname, keyword));
            lexer_list.back().AddResultFunction(callback);
        }

        const Vec<nemesis::AnimVarPtr::Lexer>& GetLexerList() const noexcept
        {
            return lexer_list;
        }

        bool IsMatch(const std::string& line) const noexcept
        {
            return line.find(keyword) != NOT_FOUND;
        }
    };

}

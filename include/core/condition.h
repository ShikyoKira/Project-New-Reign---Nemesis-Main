#pragma once

#include <regex>

#include "base/file.h"

#include "core/animvarptr.h"

#include "utilities/conditioninfo.h"

namespace nemesis
{
    struct ScopeInfo;
    struct Template;

    struct Condition : std::enable_shared_from_this<nemesis::Condition>
    {
    private:
        struct Lexer
        {
            enum Type
            {
                SYM  = 1 << 0,
                OPRN = 1 << 1,
                CPRN = 1 << 2,
                EQL  = 1 << 3,
                NEQL = 1 << 4,
                AND  = 1 << 5,
                OR   = 1 << 6,
                NOT  = 1 << 7,
                ALL  = ((1 << 8) - 1)
            };

            using Iter = Vec<std::pair<Lexer::Type, std::string>>;

            Iter tokens;

            Lexer(const std::string expression);

        private:
            static UMap<std::string, Type> lexingmap;
            static const std::regex lexingrgx;
            static const std::sregex_token_iterator end;
        };

        struct Parser
        {
            enum struct ErrorType
            {
                NONE = 0,
                UNCLOSED_BRACKET = 1180,
                SYNTAX_ERROR = 1223
            };

        private:
            bool negative = false;
            UPtr<nemesis::AnimVarPtr> varA;
            UPtr<nemesis::AnimVarPtr> varB;
            Vec<UPtr<nemesis::Condition>> and_conditions;
            Vec<UPtr<nemesis::Condition>> or_conditions;

            Vec<UPtr<nemesis::Condition>>* condition_ptr = nullptr;
            Lexer::Iter nestedtokens;
            ErrorType errortype = ErrorType::NONE;

        public:
            Parser(const Lexer& lexer, Condition& condition);
            Parser(const Lexer::Iter& tokens, Condition& condition);

            void CopyTo(Condition& condition);

        private:
            void GetNestedTokens(const Lexer::Iter& tokens, size_t& offset);
            void Compare(const Lexer::Iter& tokens, size_t& offset, Condition& condition);
            void FollowingExpression(const Lexer::Iter& tokens, size_t& offset, Condition& condition);
            void OpenParenthesis(const Lexer::Iter& tokens, size_t& offset);
            void And(const Lexer::Iter& tokens, size_t& offset, Condition& condition);
            void And(Condition& condition);
            void Or(const Lexer::Iter& tokens, size_t& offset, Condition& condition);
            void Or(Condition& condition);
        };

        bool negative = false;
        UPtr<nemesis::AnimVarPtr> varA;
        UPtr<nemesis::AnimVarPtr> varB;
        Vec<UPtr<nemesis::Condition>> and_conditions;
        Vec<UPtr<nemesis::Condition>> or_conditions;
        std::string expression;

        const nemesis::File* fileptr;
        nemesis::Line refline;

        CondType type;

    public:
        Condition(const std::string& expression, const nemesis::Line& line, const nemesis::File& file, CondType type);
        Condition(const Lexer::Iter& tokens, Condition& condition);

        const nemesis::AnimVarPtr& GetVariableA() const;
        const nemesis::AnimVarPtr& GetVariableB() const;
        std::string GetExpression() const;
        size_t GetLineNum() const;
        const nemesis::File& GetFile() const;
        CondType GetType() const;

        bool IsTrue(nemesis::ScopeInfo& scopeinfo) const;

    private:
        bool IsCurrentTrue(nemesis::ScopeInfo& scopeinfo) const;
        bool IsAndTrue(nemesis::ScopeInfo& scopeinfo) const;
        bool IsOrTrue(nemesis::ScopeInfo& scopeinfo) const;
        void TrimCondition(std::string& condition);
    };
}

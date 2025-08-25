#pragma once

#include <regex>

#include "Core/Statement/OptionStatement.h"
#include "Core/Statement/OptionVariableStatement.h"

#include "Utilities/NonCopyable.h"

namespace nemesis
{
    struct AnimationRequest;
    struct CompileState;
    struct Template;

    struct ConditionalStatement : public nemesis::Statement
    {
    private:
        struct ConditionalAnimationRequestComparer;
        struct ConditionalStringComparer;
        struct ConditionOptionComparer;
        struct ConditionalCollection;

        struct ConditionSyntaxError : nemesis::StatementException
        {
            ConditionSyntaxError(const std::string& msg);
        };

        struct ConditionalNode
        {
        protected:
            bool Negative = false;

        public:
            virtual ~ConditionalNode() noexcept {}
            virtual std::string GetExpression() const               = 0;
            virtual bool IsTrue(nemesis::CompileState& state) const = 0;

            virtual nemesis::ConditionalStatement::ConditionalCollection*
            And(nemesis::ConditionalStatement::ConditionalNode* node);
            virtual nemesis::ConditionalStatement::ConditionalCollection*
            Or(nemesis::ConditionalStatement::ConditionalNode* node);
        };

        struct ConditionalString : public nemesis::CompositeStatement, public nemesis::NonCopyableStruct
        {
        private:
            std::string ConstantValue;
            VecStr Tokens;
            SPtr<nemesis::LineModifier> Modifier;

        public:
            ConditionalString(const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              const nemesis::SemanticManager& manager);

            nemesis::ConditionalStatement::ConditionalStringComparer*
            EqualsTo(ConditionalString* term) noexcept;
            nemesis::ConditionalStatement::ConditionalStringComparer*
            NotEqualsTo(ConditionalString* term) noexcept;

            std::string GetValue(nemesis::CompileState& state) const;
        };

        struct ConditionalBoolean : public nemesis::ConditionalStatement::ConditionalNode,
                                    public nemesis::CompositeStatement,
                                    public nemesis::NonCopyableStruct
        {
        private:
            mutable std::move_only_function<bool(nemesis::CompileState&)> IsTrueFunction;

            void Parse1Component(const nemesis::TemplateClass* templt_class,
                                 const nemesis::SemanticManager& manager);
            void Parse2Components(const nemesis::TemplateClass* templt_class,
                                  const nemesis::SemanticManager& manager);
            void Parse3Components(const nemesis::TemplateClass* templt_class,
                                  const nemesis::SemanticManager& manager);
            void Parse4Components(const nemesis::TemplateClass* templt_class,
                                  const nemesis::SemanticManager& manager);

        public:
            ConditionalBoolean(const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager,
                               bool negative);

            std::string Serialize() const override;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ConditionalAnimationRequest : public nemesis::Statement, public nemesis::NonCopyableStruct
        {
        private:
            std::function<const nemesis::AnimationRequest&(nemesis::CompileState&)> GetRequestFunction;

        public:
            ConditionalAnimationRequest(const std::string& expression,
                                        size_t linenum,
                                        const std::filesystem::path& filepath,
                                        const nemesis::SemanticManager& manager);

            std::string Serialize() const override;

            nemesis::ConditionalStatement::ConditionalAnimationRequestComparer*
            EqualsTo(ConditionalAnimationRequest* request) noexcept;
            nemesis::ConditionalStatement::ConditionalAnimationRequestComparer*
            NotEqualsTo(ConditionalAnimationRequest* request) noexcept;

            const nemesis::AnimationRequest& GetRequest(nemesis::CompileState& state) const;

            static bool IsAnimationRequest(const std::string& term,
                                           const nemesis::TemplateClass& templt_class,
                                           size_t linenum,
                                           const std::filesystem::path& filepath);
        };

        struct ConditionalOption : public nemesis::OptionStatement, public nemesis::NonCopyableStruct
        {
        public:
            ConditionalOption(const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              const nemesis::SemanticManager& manager);

            nemesis::ConditionalStatement::ConditionOptionComparer*
            EqualsTo(ConditionalOption* option) noexcept;
            nemesis::ConditionalStatement::ConditionOptionComparer*
            NotEqualsTo(ConditionalOption* option) noexcept;

            static bool IsOption(const std::string& term,
                                 const nemesis::TemplateClass& templt_class,
                                 size_t linenum,
                                 const std::filesystem::path& filepath);
        };

        struct ConditionOptionComparer : public nemesis::ConditionalStatement::ConditionalNode,
                                         public nemesis::NonCopyableStruct
        {
        private:
            mutable std::string Expression;

            ConditionalOption* Variable1;
            ConditionalOption* Variable2;

        public:
            ConditionOptionComparer(ConditionalOption* variable1,
                                    ConditionalOption* variable2,
                                    bool negative) noexcept;
            ~ConditionOptionComparer() noexcept;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ConditionalAnimationRequestComparer : public nemesis::ConditionalStatement::ConditionalNode,
                                                     public nemesis::NonCopyableStruct
        {
        private:
            mutable std::string Expression;

            ConditionalAnimationRequest* Variable1;
            ConditionalAnimationRequest* Variable2;

        public:
            ConditionalAnimationRequestComparer(ConditionalAnimationRequest* variable1,
                                                ConditionalAnimationRequest* variable2,
                                       bool negative) noexcept;
            ~ConditionalAnimationRequestComparer() noexcept;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ConditionalStringComparer : public nemesis::ConditionalStatement::ConditionalNode,
                                           public nemesis::NonCopyableStruct
        {
        private:
            mutable std::string Expression;

            ConditionalString* First;
            ConditionalString* Second;

        public:
            ConditionalStringComparer(ConditionalString* first,
                                      ConditionalString* second,
                                      bool negative) noexcept;
            ~ConditionalStringComparer() noexcept;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ConditionalCollection : public nemesis::ConditionalStatement::ConditionalNode,
                                       public nemesis::NonCopyableStruct
        {
        private:
            mutable std::string Expression;

            Vec<nemesis::ConditionalStatement::ConditionalNode*> AndConditions;
            Vec<nemesis::ConditionalStatement::ConditionalNode*> OrConditions;

            bool AndIsTrue(nemesis::CompileState& state) const;

        public:
            ~ConditionalCollection() noexcept;

            nemesis::ConditionalStatement::ConditionalCollection*
            And(nemesis::ConditionalStatement::ConditionalNode* node) override;
            nemesis::ConditionalStatement::ConditionalCollection*
            Or(nemesis::ConditionalStatement::ConditionalNode* node) override;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ConditionalParentheses : public nemesis::ConditionalStatement::ConditionalNode,
                                        public nemesis::NonCopyableStruct
        {
        private:
            mutable std::string Expression;

            nemesis::ConditionalStatement::ConditionalNode* SubNode;

        public:
            ConditionalParentheses(nemesis::ConditionalStatement::ConditionalNode* subnode,
                                   bool negative) noexcept;
            ~ConditionalParentheses() noexcept;

            std::string GetExpression() const override;

            bool IsTrue(nemesis::CompileState& state) const override;
        };

        class ConditionalStatementParser
        {
        private:
            enum TokenType
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

            struct Token
            {
                TokenType Type;
                std::string Value;

                Token(TokenType type, const std::string& value) noexcept;
            };

            struct TokenParser
            {
                std::sregex_token_iterator End;
                std::regex TokenRegex;
                UMap<std::string, TokenType> TokenMap;

                TokenParser();
            };

            Vec<nemesis::ConditionalStatement::ConditionalStatementParser::Token> Tokens;
            size_t LineNum;
            const std::filesystem::path* FilePathPtr;
            const nemesis::SemanticManager& SemanticManager;

            mutable size_t TokenIndex = 0;

            const nemesis::ConditionalStatement::ConditionalStatementParser::Token& Advance() const noexcept;
            const nemesis::ConditionalStatement::ConditionalStatementParser::Token& Previous() const noexcept;
            bool
            Match(nemesis::ConditionalStatement::ConditionalStatementParser::TokenType type) const noexcept;
            const nemesis::ConditionalStatement::ConditionalStatementParser::Token& Consume(TokenType type) const;
            bool IsEnd() const noexcept;

            nemesis::ConditionalStatement::ConditionalNode* ParseExpression() const;
            nemesis::ConditionalStatement::ConditionalNode* ParseTerm() const;
            nemesis::ConditionalStatement::ConditionalNode* ParseFactor() const;

        public:
            ConditionalStatementParser(const nemesis::SemanticManager& manager) noexcept;

            void SetExpression(const std::string& expression);
            void SetLineNumber(size_t linenum) noexcept;
            void SetFilePath(const std::filesystem::path& filepath) noexcept;

            SPtr<nemesis::ConditionalStatement::ConditionalNode> MakeCondition() const;
        };

    protected:
        SPtr<nemesis::ConditionalStatement::ConditionalNode> CondNode;

        ConditionalStatement(const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);
        ConditionalStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager);
        ConditionalStatement(const nemesis::ConditionalStatement& statement);

    public:
        virtual ~ConditionalStatement() noexcept {}
        virtual std::string Serialize() const = 0;

        virtual bool IsTrue(nemesis::CompileState& state) const = 0;
    };
}

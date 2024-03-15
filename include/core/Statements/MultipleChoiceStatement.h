#pragma once

#include "core/Statements/ConditionalStatement.h"

namespace nemesis
{
    struct LineModifier;

	struct MultipleChoiceStatement : public nemesis::Statement
    {
    private:
        struct ChoiceCondition : public nemesis::ConditionalStatement
        {
            ChoiceCondition(const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);

            std::string Serialize() const override;
            bool IsTrue(nemesis::CompileState& state) const override;
        };

        struct ChoiceValue
        {
        private:
            std::string Expression;
            Map<size_t, Vec<SPtr<nemesis::LineModifier>>> Modifiers;

        public:
            ChoiceValue(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);

            const std::string& GetExpression() const;
            std::string GetValue(nemesis::CompileState& state) const;
        };

        struct Choice
        {
        private:
            size_t Begin;
            size_t End;
            UPtr<nemesis::MultipleChoiceStatement::ChoiceCondition> Condition;
            UPtr<nemesis::MultipleChoiceStatement::ChoiceValue> Value;

        public:
            Choice(size_t begin,
                   size_t end,
                   const std::string& expression,
                   size_t linenum,
                   const std::filesystem::path& filepath,
                   const nemesis::SemanticManager& manager,
                   const std::string& value);

            const nemesis::MultipleChoiceStatement::ChoiceCondition* GetCondition() const;
            const nemesis::MultipleChoiceStatement::ChoiceValue* GetValue() const;
            bool IsBetween(size_t begin, size_t end) const;
        };

        Vec<nemesis::MultipleChoiceStatement::Choice> Choices;

    public:
        MultipleChoiceStatement(const std::string& line,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(const VecStr blocks, nemesis::CompileState& state) const;

        bool IsPartOfChoiceValue(size_t begin, size_t end) const;
    };
}

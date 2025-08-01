#include <regex>

#include "Core/Statement//MultipleChoiceStatement.h"

#include "Core/LineModifier/LineModifier.h"
#include "Core/LineModifier/MultipleChoiceModifier.h"

nemesis::MultipleChoiceStatement::ChoiceCondition::ChoiceCondition(const std::string& expression,
                                                                   size_t linenum,
                                                                   const std::filesystem::path& filepath,
                                                                   const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::MultipleChoiceStatement::ChoiceCondition::Serialize() const
{
    return Expression;
}

bool nemesis::MultipleChoiceStatement::ChoiceCondition::IsTrue(nemesis::CompileState& state) const
{
    return CondNode->IsTrue(state);
}

nemesis::MultipleChoiceStatement::ChoiceValue::ChoiceValue(const std::string& expression,
                                                           size_t linenum,
                                                           const std::filesystem::path& filepath,
                                                           const nemesis::SemanticManager& manager)
{
    Expression = expression;
    Modifiers  = nemesis::LineModifierFactory::BuildModifiers(Expression, linenum, filepath, manager);

    for (auto& modifier_list : Modifiers)
    {
        for (auto& modifier : modifier_list.second)
        {
            if (!dynamic_cast<nemesis::MultipleChoiceModifier*>(modifier.get())) continue;

            throw std::runtime_error("Only 1 MultiChoice per line");
        }
    }
}

const std::string& nemesis::MultipleChoiceStatement::ChoiceValue::GetExpression() const
{
    return Expression;
}

std::string nemesis::MultipleChoiceStatement::ChoiceValue::GetValue(nemesis::CompileState& state) const
{
    if (Modifiers.empty()) return Expression;

    VecStr tokens;
    std::string merged;

    for (auto& ch : Expression)
    {
        tokens.emplace_back(1, ch);
    }

    for (auto& modifier_list : Modifiers)
    {
        for (auto& mod : modifier_list.second)
        {
            mod->Apply(tokens, state);
        }
    }

    for (auto& token : tokens)
    {
        merged.append(token);
    }

    return merged;
}

nemesis::MultipleChoiceStatement::Choice::Choice(size_t begin,
                                                 size_t end,
                                                 const std::string& expression,
                                                 size_t linenum,
                                                 const std::filesystem::path& filepath,
                                                 const nemesis::SemanticManager& manager,
                                                 const std::string& value)
    : Begin(begin)
    , End(end)
{
    if (value.empty()) throw std::runtime_error("Choice value cannot be empty");

    Value = std::make_unique<ChoiceValue>(value, linenum, filepath, manager);

    if (expression.empty()) return;

    Condition = std::make_unique<ChoiceCondition>(expression, linenum, filepath, manager);
}

const nemesis::MultipleChoiceStatement::ChoiceCondition*
nemesis::MultipleChoiceStatement::Choice::GetCondition() const
{
    return Condition.get();
}

const nemesis::MultipleChoiceStatement::ChoiceValue*
nemesis::MultipleChoiceStatement::Choice::GetValue() const
{
    return Value.get();
}

bool nemesis::MultipleChoiceStatement::Choice::IsBetween(size_t begin, size_t end) const
{
    return Begin <= begin && end <= End;
}

nemesis::MultipleChoiceStatement::MultipleChoiceStatement(const std::string& line,
                                                          size_t linenum,
                                                          const std::filesystem::path& filepath,
                                                          const nemesis::SemanticManager& manager)
    : nemesis::Statement(line, linenum, filepath, true)
{
    std::smatch nmatch;

    if (!std::regex_search(line, nmatch, std::regex("[\\t|\\s]+(<!--\\s.+\\s-->)$")))
    {
        ThrowSyntaxError("Missing component for MultipleChoice");
    }

    Expression = nmatch.str(1);

    static const std::regex pattern("<!--\\s(?:\\^(.+?)\\^\\s|)(.+?)\\s-->");
    static const std::sregex_iterator end;

    try
    {
        for (std::sregex_iterator itr(Expression.begin(), Expression.end(), pattern); itr != end; ++itr)
        {
            size_t pos      = nmatch.position(1) + itr->position(2);
            std::string val = itr->str(2);
            Choices.emplace_back(pos, pos + val.size(), itr->str(1), linenum, filepath, manager, val);
            Components.emplace_back(itr->str(0));
        }
    }
    catch (const std::runtime_error& ex)
    {
        ThrowSyntaxError(ex.what());
    }
}

std::string nemesis::MultipleChoiceStatement::Serialize() const
{
    auto& first_choice    = Choices.front();
    std::string statement = "<!-- ";
    auto* first_condition = first_choice.GetCondition();

    if (first_condition && !first_condition->GetExpression().empty())
    {
        statement += first_condition->GetExpression() + " ";
    }

    statement += first_choice.GetValue()->GetExpression() + " -->";

    for (size_t i = 1; i < Choices.size(); ++i)
    {
        statement += " <!-- ";
        auto& choice    = Choices[i];
        auto* condition = choice.GetCondition();

        if (condition && !condition->GetExpression().empty())
        {
            statement += condition->GetExpression() + " ";
        }

        statement += choice.GetValue()->GetExpression() + " -->";
    }

    return statement;
}

std::string nemesis::MultipleChoiceStatement::GetValue(nemesis::CompileState& state) const
{
    for (auto& choice : Choices)
    {
        auto* condition = choice.GetCondition();

        // without condition will be treated as ELSE
        if (condition && !condition->IsTrue(state)) continue;

        return choice.GetValue()->GetValue(state);
    }

    return "";
}

bool nemesis::MultipleChoiceStatement::IsPartOfChoiceValue(size_t begin, size_t end) const
{
    for (auto& choice : Choices)
    {
        if (!choice.IsBetween(begin, end)) continue;

        return true;
    }

    return false;
}

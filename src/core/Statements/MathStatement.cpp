#include "core/Statements/MathStatement.h"

#include "core/LineModifier/LineModifier.h"

#include "utilities/compute.h"

nemesis::MathStatement::MathStatement(const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    if (Components.size() != 2) throw std::runtime_error("Math only accepts 1 argument");

    auto& component = Components.back();
    size_t close_pos, pos = component.find("{");

    if (pos == NOT_FOUND)
    {
        if (component.find("}", pos) != NOT_FOUND) throw std::runtime_error("Syntax error: Unopened '}'");

        GetEquationFunction = [&component](nemesis::CompileState& state) { return component; };
        return;
    }

    for (auto& ch : component)
    {
        Tokens.emplace_back(1, ch);
    }

    do
    {
        close_pos = component.find("}", pos);

        if (close_pos == NOT_FOUND) throw std::runtime_error("Syntax error: Unclosed '{'");

        std::string cur = component.substr(pos + 1, close_pos - pos - 1);
        auto u_modifier = nemesis::LineModifierFactory::BuildModifier(
            pos, close_pos, cur, component, linenum, filepath, manager);
        Modifiers.emplace_back(std::move(u_modifier));

        pos = component.find("{", close_pos);
    } while (pos != NOT_FOUND);

    if (component.find("}", close_pos + 1) != NOT_FOUND)
    {
        throw std::runtime_error("Syntax error: Unopened '}'");
    }

    GetEquationFunction = [this](nemesis::CompileState& state)
    {
        VecStr tokens(Tokens);
        std::string merged;

        for (auto& modifier : Modifiers)
        {
            modifier->Apply(tokens, state);
        }

        for (auto& token : tokens)
        {
            merged.append(token);
        }

        return merged;
    };
}

std::string nemesis::MathStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::MathStatement::GetValue(nemesis::CompileState& state) const
{
    std::string result, equation = GetEquationFunction(state);
    
    if (nemesis::calculate(equation, result)) return result;

    throw std::runtime_error("Unsupported math equation (Equation: " + equation
                             + ", Line: " + std::to_string(LineNum) + ", File: " + FilePath.string() + ")");
}

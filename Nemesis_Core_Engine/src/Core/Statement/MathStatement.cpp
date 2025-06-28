#include "Core/Statement/MathStatement.h"

#include "Core/LineModifier/LineModifier.h"

#include "Utilities/Algorithm.h"
#include "Utilities/Compute.h"

nemesis::MathStatement::MathStatement(const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 2)
    {
        ThrowSyntaxError("Math only accepts 1 argument");
    }

    DynamicComponents.emplace_back(Components.back(), linenum, filepath, manager);
}

std::string nemesis::MathStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::MathStatement::GetValue(nemesis::CompileState& state) const
{
    std::string result, equation = DynamicComponents.front().GetValue(state);

    if (!nemesis::calculate(equation, result, true))
    {
        ThrowSyntaxError("Invalid math equation");
    }

    if (equation.find(".") != NOT_FOUND || !nemesis::ends_with(result, ".000000")) return result;

    return result.substr(0, result.size() - 7);
}

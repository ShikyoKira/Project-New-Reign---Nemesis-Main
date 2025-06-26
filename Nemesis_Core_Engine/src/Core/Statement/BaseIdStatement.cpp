#include "Core/Statement/BaseIdStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/LineModifier/LineModifier.h"

#include "Utilities/Algorithm.h"

std::string nemesis::BaseIdStatement::GetComponentValue(nemesis::CompileState& state) const
{
    if (DynamicComponents.empty()) return Components.back();

    return DynamicComponents.back().GetValue(state);
}

std::string nemesis::BaseIdStatement::GetTypeName() const
{
    return "";
}

nemesis::BaseIdStatement::BaseIdStatement(const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 2)
    {
        throw std::runtime_error("Syntax Error: " + GetTypeName() + " only accepts 1 argument (Syntax: "
                                 + expression + ", Line: " + std::to_string(linenum)
                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    auto& component = Components.back();

    if (!IsComplexComponent(component)) return;

    DynamicComponents.emplace_back(component, linenum, filepath, manager);
}

std::string nemesis::BaseIdStatement::Serialize() const
{
    return Expression;
}

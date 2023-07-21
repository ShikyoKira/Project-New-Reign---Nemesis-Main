#include "core/Statements/VariableIdStatement.h"

#include "core/CompileState.h"

#include "core/LineModifier/LineModifier.h"

std::string nemesis::VariableIdStatement::GetTypeName() const
{
    return "VariableID";
}

nemesis::VariableIdStatement::VariableIdStatement(const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::BaseIdStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::VariableIdStatement::GetValue(nemesis::CompileState& state) const
{
    std::string component = GetComponentValue(state);
    return state.GetAttributeID(component);
}

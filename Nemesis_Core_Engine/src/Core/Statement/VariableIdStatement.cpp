#include "Core/Statement/VariableIdStatement.h"

#include "Core/CompileState.h"

#include "Core/LineModifier/LineModifier.h"

#include "Utilities/Algorithm.h"

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
    std::string out;

    if (state.TryGetVariableID(component, out)) return out;

    throw std::runtime_error("Variable id not found (Name: " + component
                             + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ") ");
}

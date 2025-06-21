#include "Core/Statement/PropertyIdStatement.h"

#include "Core/CompileState.h"

#include "Core/LineModifier/LineModifier.h"

std::string nemesis::PropertyIdStatement::GetTypeName() const
{
    return "PropertyID";
}

nemesis::PropertyIdStatement::PropertyIdStatement(const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::BaseIdStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::PropertyIdStatement::GetValue(nemesis::CompileState& state) const
{
    std::string component = GetComponentValue(state);
    return state.GetPropertyID(component);
}

#include "core/Statements/AttributeIdStatement.h"

#include "core/CompileState.h"

#include "core/LineModifier/LineModifier.h"

std::string nemesis::AttributeIdStatement::GetTypeName() const
{
    return "AttributeID";
}

nemesis::AttributeIdStatement::AttributeIdStatement(const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::BaseIdStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::AttributeIdStatement::GetValue(nemesis::CompileState& state) const
{
    std::string component = GetComponentValue(state);
    return state.GetAttributeID(component);
}

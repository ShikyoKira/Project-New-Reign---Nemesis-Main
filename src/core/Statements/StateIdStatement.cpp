#include "core/Statements/StateIdStatement.h"

#include "core/CompileState.h"

#include "core/LineModifier/LineModifier.h"

std::string nemesis::StateIdStatement::GetTypeName() const
{
    return "StateID";
}

nemesis::StateIdStatement::StateIdStatement(const std::string& expression,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            const nemesis::SemanticManager& manager)
    : nemesis::BaseIdStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::StateIdStatement::GetValue(nemesis::CompileState& state) const
{
    std::string component = GetComponentValue(state);
    return state.GetStateID(component);
}

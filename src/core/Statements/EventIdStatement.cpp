#include "core/Statements/EventIdStatement.h"

#include "core/CompileState.h"

#include "core/LineModifier/LineModifier.h"

std::string nemesis::EventIdStatement::GetTypeName() const
{
    return "EventID";
}

nemesis::EventIdStatement::EventIdStatement(const std::string& expression,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            const nemesis::SemanticManager& manager)
    : nemesis::BaseIdStatement(expression, linenum, filepath, manager)
{
}

std::string nemesis::EventIdStatement::GetValue(nemesis::CompileState& state) const
{
    std::string component = GetComponentValue(state);
    return state.GetEventID(component);
}

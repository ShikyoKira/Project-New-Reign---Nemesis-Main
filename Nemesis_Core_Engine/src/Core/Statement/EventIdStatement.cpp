#include "Core/Statement/EventIdStatement.h"

#include "Core/CompileState.h"

#include "Core/LineModifier/LineModifier.h"

#include "Utilities/Algorithm.h"

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
    std::string out;

    if (state.TryGetEventID(component, out)) return out;

    throw std::runtime_error("Invalid Value: Event id not found (Name: " + component
                             + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ") ");
}

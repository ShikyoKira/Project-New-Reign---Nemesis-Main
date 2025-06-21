#include "Core/Statement/AttributeIdStatement.h"

#include "Core/CompileState.h"

#include "Core/LineModifier/LineModifier.h"

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
    std::string out;

    if (state.TryGetAttributeID(component, out)) return out;

    throw std::runtime_error("Attribute id not found (Name: " + component + ", File: "
                             + FilePath.string() + ", Line: " + std::to_string(LineNum) + ") ");
}

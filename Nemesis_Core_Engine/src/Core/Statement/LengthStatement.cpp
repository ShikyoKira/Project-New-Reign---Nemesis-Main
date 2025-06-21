#include "Core/Statement/LengthStatement.h"

nemesis::LengthStatement::LengthStatement(const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 2)
    {
        throw std::runtime_error("Syntax Error: Length only accepts 1 argument (Syntax: " + expression
                                 + ", Line: " + std::to_string(linenum) + ", File: " + filepath.string()
                                 + ")");
    }

    auto& value = Components.back();

    if (!IsComplexComponent(value)) return;

    DynamicComponents.emplace_back(value, linenum, filepath, manager);
}

std::string nemesis::LengthStatement::GetValue(nemesis::CompileState& state) const
{
    if (DynamicComponents.empty()) return std::to_string(Components.back().length());

    return std::to_string(DynamicComponents.back().GetValue(state).length());
}

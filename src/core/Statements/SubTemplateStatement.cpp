#include "core/Statements/SubTemplateStatement.h"

#include "core/LineModifier/LineModifier.h"

nemesis::SubTemplateStatement::SubTemplateStatement(const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    for (size_t i = 1; i < Components.size(); ++i)
    {
        DynamicComponents.emplace_back(Components[i], linenum, filepath, manager);
    }
}

std::string nemesis::SubTemplateStatement::Serialize() const
{
    return Expression;
}

VecStr nemesis::SubTemplateStatement::GetComponents(nemesis::CompileState& state) const
{
    VecStr components;
    std::string value = Components.front();

    for (auto& component : DynamicComponents)
    {
        value.append("[" + components.emplace_back(component.GetValue(state)) + "]");
    }
    
    components.insert(components.begin(), value);
    return components;
}

std::string nemesis::SubTemplateStatement::GetValue(nemesis::CompileState& state) const
{
    std::string value = Components.front();

    for (auto& component : DynamicComponents)
    {
        value.append("[" + component.GetValue(state) + "]");
    }

    return value;
}

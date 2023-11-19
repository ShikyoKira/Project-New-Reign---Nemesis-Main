#include "core/Template/TemplateOption.h"
#include "core/Template/TemplateOptionModel.h"

nemesis::TemplateOption::TemplateOption(const std::string& expression,
                                        const std::string& name)
    : Expression(expression)
    , Name(name)
{
}

nemesis::TemplateOption::TemplateOption(const std::string& expression,
                                        const std::string& name,
                                        const VecStr& aliases)
    : Expression(expression)
    , Name(name)
    , Aliases(aliases)
{
}

const std::string& nemesis::TemplateOption::GetExpression() const
{
    return Expression;
}

const std::string& nemesis::TemplateOption::GetName() const
{
    return Name;
}

const VecStr& nemesis::TemplateOption::GetChildren() const
{
    return Aliases;
}

bool nemesis::TemplateOption::HasAlias(const std::string& alias) const
{
    for (auto& a : Aliases)
    {
        if (a == alias) return true;
    }

    return false;
}

const std::string& nemesis::TemplateOption::GetVariableValue(const std::string& name) const
{
    auto itr = Variables.find(name);

    if (itr != Variables.end()) return itr->second;

    throw std::runtime_error("Option variable cannot be found (Variable: " + name + ")");
}

void nemesis::TemplateOption::AddVariable(const std::string& name, const std::string& value)
{
    Variables[name] = value;
}

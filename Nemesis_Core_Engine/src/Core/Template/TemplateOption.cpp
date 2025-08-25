#include "Core/Template/TemplateOption.h"
#include "Core/Template/TemplateOptionModel.h"

nemesis::TemplateOption::TemplateOption(const std::string& expression,
                                        const std::string& expression_name,
                                        const nemesis::TemplateOptionModel& model)
    : Expression(expression)
    , ExpressionName(expression_name)
    , Model(model)
{
}

const std::string& nemesis::TemplateOption::GetExpression() const
{
    return Expression;
}

const std::string& nemesis::TemplateOption::GetName() const
{
    return Model.GetName();
}

const std::string& nemesis::TemplateOption::GetExpressionName() const
{
    return ExpressionName;
}

const VecStr& nemesis::TemplateOption::GetAliases() const
{
    return Model.GetAliases();
}

bool nemesis::TemplateOption::HasAlias(const std::string& alias) const
{
    for (auto& a : Model.GetAliases())
    {
        if (a == alias) return true;
    }

    return false;
}

const std::string& nemesis::TemplateOption::GetVariableValue(const std::string& name) const
{
    auto itr = Variables.find(name);

    if (itr != Variables.end()) return itr->second;

    throw std::runtime_error("Option variable \"" + name + "\" cannot be found");
}

void nemesis::TemplateOption::AddVariable(const std::string& name, const std::string& value)
{
    Variables[name] = value;
}

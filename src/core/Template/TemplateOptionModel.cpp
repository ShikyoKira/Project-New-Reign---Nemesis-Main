#include "core/Template/TemplateClass.h"
#include "core/Template/TemplateOptionModel.h"

#include "utilities/stringextension.h"

nemesis::TemplateOptionModel::TemplateOptionModel(const std::string name,
                                                  const VecStr& aliases,
                                                  const VecStr& variables,
                                                  bool b_array)
    : Name(name)
    , Aliases(aliases)
    , Variables(variables)
    , bArray(b_array)
{
    OrderedNames.insert(Variables.begin(), Variables.end());
    OrderedNames.insert(Name);
}

const std::string& nemesis::TemplateOptionModel::GetName() const noexcept
{
    return Name;
}

bool nemesis::TemplateOptionModel::HasAlias(const std::string& alias) const noexcept
{
    for (auto& each : Aliases)
    {
        if (each == alias) return true;
    }

    return false;
}

bool nemesis::TemplateOptionModel::HasVariable(const std::string& variable_name) const noexcept
{
    for (auto& variable : Variables)
    {
        if (variable != variable_name) continue;

        return true;
    }

    return false;
}

bool nemesis::TemplateOptionModel::IsArray() const noexcept
{
    return bArray;
}

UPtr<nemesis::TemplateOption> nemesis::TemplateOptionModel::TryCreateOption(
    const std::string& expression, size_t linenum, const std::filesystem::path& filepath) const
{
    if (expression == Name)
    {
        if (Variables.empty()) return std::make_unique<nemesis::TemplateOption>(expression, Name, Aliases);

        throw std::runtime_error("Missing variable for option (Option: " + Name + ", Variable: " + Variables.front() + ", Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    for (auto& name : OrderedNames)
    {
        if (!expression._Starts_with(name)) continue;

        auto option = name == Name ? std::make_unique<nemesis::TemplateOption>(expression, name, Aliases)
                                   : std::make_unique<nemesis::TemplateOption>(expression, name);
        std::stringstream ss(expression.substr(name.size()));

        for (auto& variable : Variables)
        {
            std::string option_expression;

            if (!std::getline(ss, option_expression, '/'))
            {
                throw std::runtime_error("Missing variable for option (Option: " + name
                                         + ", Variable: " + variable + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            option->AddVariable(variable, option_expression);
        }

        return option;
    }

    return nullptr;
}

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

        throw std::runtime_error("missing variable for option (Option: " + Name + ", Variable: " + Variables.front() + ", Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    if (!expression._Starts_with(Name)) return nullptr;

    auto option = std::make_unique<nemesis::TemplateOption>(expression, Name, Aliases);
    std::stringstream ss(expression.substr(Name.size()));

    for (auto& variable : Variables)
    {
        std::string option_expression;

        if (!std::getline(ss, option_expression, '/'))
        {
            throw std::runtime_error("missing variable for option (Option: " + Name
                                     + ", Variable: " + variable + ", Line: "
                                     + std::to_string(linenum) + ", File: " + filepath.string() + ")");
        }

        option->AddVariable(variable, option_expression);
    }

    return option;
}

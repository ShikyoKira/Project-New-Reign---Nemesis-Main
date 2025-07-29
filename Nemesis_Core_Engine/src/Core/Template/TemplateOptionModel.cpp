#include "Core/Template/TemplateOptionModel.h"
#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"
#include "Utilities/StringExtension.h"

void nemesis::TemplateOptionModel::AddVariablesToOption(nemesis::TemplateOption& opt,
                                                        const std::string& expr,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath) const
{
    std::istringstream iss(expr);

    for (auto& variable : Variables)
    {
        std::string opt_expr;

        if (!std::getline(iss, opt_expr, '/'))
        {
            throw std::runtime_error("Missing variable for option (Option: " + Name
                                     + ", Variable: " + variable + ", Line: " + std::to_string(linenum)
                                     + ", File: " + nemesis::to_utf8_string(filepath) + ")");
        }

        opt.AddVariable(variable, opt_expr);
    }

    if (iss.eof()) return;

    throw std::runtime_error("Unexpected trailing data after expected input (Option: " + Name
                             + ", Line: " + std::to_string(linenum)
                             + ", File: " + nemesis::to_utf8_string(filepath) + ")");
}

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

const VecStr& nemesis::TemplateOptionModel::GetAliases() const noexcept
{
    return Aliases;
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

        throw std::runtime_error("Missing variable for option (Option: " + Name
                                 + ", Variable: " + Variables.front() + ", Line: " + std::to_string(linenum)
                                 + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    if (!expression.starts_with(Name)) return nullptr;

    auto option = std::make_unique<nemesis::TemplateOption>(expression, Name, Aliases);
    AddVariablesToOption(*option, expression.substr(Name.size()), linenum, filepath);
    return option;
}

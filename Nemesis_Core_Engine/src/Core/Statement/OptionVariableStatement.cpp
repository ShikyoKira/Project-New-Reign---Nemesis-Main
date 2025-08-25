#include <sstream>

#include "Core/Statement/OptionVariableStatement.h"

#include "Core/CompileState.h"

#include "Core/LineModifier/LineModifier.h"

#include "Core/Template/TemplateClass.h"
#include "Core/Template/TemplateOption.h"

#include "Utilities/Algorithm.h"

SPtr<std::function<std::string(nemesis::CompileState&)>>
nemesis::OptionVariableStatement::GetVariableFunction(const std::string& variable,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath,
                                                      const nemesis::SemanticManager& manager,
                                                      const nemesis::TemplateOptionModel* model)
{
    if (!IsComplexComponent(variable))
    {
        if (model->HasVariable(variable))
        {
            return std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
                [variable](nemesis::CompileState& state) { return variable; });
        }

        ThrowSyntaxError("Option variable does not exist (\"" + model->GetName() + "[" + variable + "]\")");
    }

    auto& component = DynamicComponents.emplace_back(variable, linenum, filepath, manager);

    return std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
        [this, model, &component](nemesis::CompileState& state)
        {
            std::string option_var = component.GetValue(state);

            if (model->HasVariable(option_var)) return option_var;

            ThrowSyntaxError("Option variable does not exist (\"" + model->GetName() + "[" + option_var
                             + "]\")");
        });
}

nemesis::OptionVariableStatement::OptionVariableStatement(const std::string& expression,
                                                          size_t linenum,
                                                          const std::filesystem::path& filepath,
                                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
    , OptionStatement(expression.substr(0, expression.rfind("[")), linenum, filepath, manager)
{
    SPtr<std::function<std::string(nemesis::CompileState&)>> get_option_var;
    const std::string *name_ptr, *var_name_ptr;

    auto template_class = manager.GetCurrentTemplateClass();

    switch (Components.size())
    {
        case 2:
        case 3:
        {
            name_ptr     = &Components.front();
            var_name_ptr = &Components.back();
            break;
        }
        case 4:
        {
            name_ptr     = &Components[2];
            var_name_ptr = &Components[3];
            break;
        }
        case 5:
        {
            name_ptr     = &Components[2];
            var_name_ptr = &Components[4];
            break;
        }
        default:
            ThrowSyntaxError("Invalid option components");
    }

    auto model     = template_class->GetModel(*name_ptr);
    get_option_var = GetVariableFunction(*var_name_ptr, linenum, filepath, manager, model);
    SPtr<std::function<std::string(const Pair<const nemesis::TemplateOption*, int>&, nemesis::CompileState&)>>
        get_value = std::make_shared<
            std::function<std::string(const Pair<const nemesis::TemplateOption*, int>&, nemesis::CompileState&)>>(
            [this, get_option_var](const Pair<const nemesis::TemplateOption*, int>& pair,
                                   nemesis::CompileState& state)
            {
                if (pair.second != -1) return std::to_string(pair.second);

                auto* option = pair.first;

                if (!option) ThrowInaccessibleError("Failed to access required option");

                return option->GetVariableValue((*get_option_var)(state));
            });

    GetValueFunction = [this, get_value](nemesis::CompileState& state)
    {
        auto pair_list = OptionStatement.GetValues(state);
        std::ostringstream oss;
        size_t i = 0;

        for (; i < pair_list.size(); ++i)
        {
            std::string str = (*get_value)(pair_list[i], state);

            if (str.empty()) continue;

            oss << str;
            break;
        }

        for (++i; i < pair_list.size(); ++i)
        {
            std::string str = (*get_value)(pair_list[i], state);

            if (str.empty()) continue;

            oss << ' ' << str;
        }

        return oss.str();
    };
}

std::string nemesis::OptionVariableStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::OptionVariableStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

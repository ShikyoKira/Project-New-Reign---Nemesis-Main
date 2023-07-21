#include "core/Statements/OptionVariableStatement.h"

#include "utilities/template.h"
#include "utilities/templatecategory.h"

#include "core/CompileState.h"

#include "core/LineModifier/LineModifier.h"

#include "core/Template/TemplateClass.h"
#include "core/Template/TemplateOption.h"

SPtr<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*, nemesis::CompileState&)>>
nemesis::OptionVariableStatement::GetOptionFunction(const std::string& option_syntax,
                                                    const std::string& option_name,
                                                    const std::string& index,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
{
    if (index.empty())
    {
        if (!manager.HasOptionInQueue(option_syntax))
        {
            throw std::runtime_error("Syntax error: Option not in queue (Line: " + std::to_string(linenum)
                                     + ", File: " + filepath.string() + ")");
        }

        return std::make_shared<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*,
                                                                             nemesis::CompileState&)>>(
            [&option_name](const nemesis::AnimationRequest* request, nemesis::CompileState& state)
            { return state.GetCurrentRequestOption(request, option_name); });
    }

    if (!IsComplexComponent(index))
    {
        if (index == "F")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, &option_name](const nemesis::AnimationRequest* request, nemesis::CompileState& state)
                {
                    auto options = request->GetOptions(option_name);

                    if (!options.empty()) return options.front();
                    
                    throw std::runtime_error("Option is required (\"" + option_name + "\")");
                });
        }
        
        if (index == "L")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, &option_name](const nemesis::AnimationRequest* request, nemesis::CompileState& state)
                {
                    auto options = request->GetOptions(option_name);

                    if (!options.empty()) return options.back();
                    
                    throw std::runtime_error("Option is required (\"" + option_name + "\")");
                });
        }
        
        if (isOnlyNumber(index))
        {
            size_t i = std::stoul(index);

            return std::make_shared<std::function<const nemesis::TemplateOption*(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, i, &option_name](const nemesis::AnimationRequest* request,
                                            nemesis::CompileState& state)
                {
                    auto options = request->GetOptions(option_name);

                    if (options.size() > i) return options[i];

                    throw std::runtime_error("Option must be more than " + std::to_string(i)
                                             + " in array (\"" + option_name + "\")");
                });
        }

        throw std::runtime_error("Syntax Error: Invalid option index (Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    auto& component = DynamicComponents.emplace_back(index, linenum, filepath, manager);

    return std::make_shared<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*,
                                                                         nemesis::CompileState&)>>(
        [this, &component, &option_name, linenum, filepath](const nemesis::AnimationRequest* request,
                                                                 nemesis::CompileState& state)
        {
            std::string s_index = component.GetValue(state);

            if (s_index.empty()) return state.GetCurrentRequestOption(request, option_name);

            auto options = request->GetOptions(option_name);

            if (s_index == "F")
            {
                if (!options.empty()) return options.front();

                throw std::runtime_error("Option is required (\"" + option_name + "\")");
            }
            
            if (s_index == "L")
            {
                if (!options.empty()) return options.back();
                
                throw std::runtime_error("Option is required (\"" + option_name + "\")");
            }
            
            if (isOnlyNumber(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index) return options[index];

                throw std::runtime_error("Option must be more than " + std::to_string(index) + " in array (\""
                                         + option_name + "\")");
            }

            throw std::runtime_error("Syntax Error: Invalid option index (Line: " + std::to_string(linenum)
                                     + ", File: " + filepath.string() + ")");
        });
}

SPtr<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>
nemesis::OptionVariableStatement::GetBaseOptionFunction(const std::string& option_name,
                                                        const std::string& index_str,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
{
    if (index_str.empty())
    {
        if (!manager.HasOptionInQueue(option_name))
        {
            throw std::runtime_error("Syntax error: Option not in queue (Line: " + std::to_string(linenum)
                                     + ", File: " + filepath.string() + ")");
        }

        return std::make_shared<
            std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
            [&option_name](nemesis::CompileState& state)
            { return state.GetCurrentOption(option_name); });
    }

    if (!IsComplexComponent(index_str))
    {
        if (index_str == "F")
        {
            return std::make_shared<
                std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [&option_name](nemesis::CompileState& state)
                {
                    auto options = state.GetBaseRequest()->GetOptions(option_name);

                    if (!options.empty()) return options.front();

                    throw std::runtime_error("Option is required (\"" + option_name + "\")");
                });
        }
        
        if (index_str == "L")
        {
            return std::make_shared<
                std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [&option_name](nemesis::CompileState& state)
                {
                    auto options = state.GetBaseRequest()->GetOptions(option_name);

                    if (!options.empty()) return options.back();

                    throw std::runtime_error("Option is required (\"" + option_name + "\")");
                });
        }
        
        if (isOnlyNumber(index_str))
        {
            size_t index = std::stoul(index_str);
            return std::make_shared<
                std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [index, &option_name](nemesis::CompileState& state)
                {
                    auto options = state.GetBaseRequest()->GetOptions(option_name);

                    if (options.size() > index) return options[index];

                    throw std::runtime_error("Option must be more than " + std::to_string(index)
                                             + " in array (\"" + option_name + "\")");
                });
        }

        throw std::runtime_error("Syntax Error: Invalid option index (Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    auto& component = DynamicComponents.emplace_back(index_str, linenum, filepath, manager);

    return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
        [&component, &option_name, linenum, filepath](nemesis::CompileState& state)
        {
            std::string s_index = component.GetValue(state);

            if (s_index.empty()) return state.GetCurrentOption(option_name);

            auto options = state.GetBaseRequest()->GetOptions(option_name);

            if (s_index == "F")
            {
                if (!options.empty()) return options.front();

                throw std::runtime_error("Option is required (\"" + option_name + "\")");
            }

            if (s_index == "L")
            {
                if (!options.empty()) return options.back();

                throw std::runtime_error("Option is required (\"" + option_name + "\")");
            }

            if (isOnlyNumber(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index) return options[index];

                throw std::runtime_error("Option must be more than " + std::to_string(index) + " in array (\""
                                         + option_name + "\")");
            }

            throw std::runtime_error("Syntax Error: Invalid option index (Line: " + std::to_string(linenum)
                                     + ", File: " + filepath.string() + ")");
        });
}

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

        throw std::runtime_error("Syntax error: Option variable does not exist (\"" + model->GetName() + "["
                                 + variable + "]\")");
    }

    auto& component = DynamicComponents.emplace_back(variable, linenum, filepath, manager);

    return std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
        [model, &component](nemesis::CompileState& state)
        {
            std::string option_var = component.GetValue(state);

            if (model->HasVariable(option_var)) return option_var;

            throw std::runtime_error("Syntax error: Option variable does not exist (\"" + model->GetName()
                                     + "[" + option_var + "]\")");
        });
}

nemesis::OptionVariableStatement::OptionVariableStatement(const std::string& expression,
                                                          size_t linenum,
                                                          const std::filesystem::path& filepath,
                                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    auto template_class = manager.GetCurrentTemplateClass();

    switch (Components.size())
    {
        case 2:
        {
            std::string& option_name = Components.front();
            std::string& option_var  = Components.back();
            auto model               = template_class->GetModel(option_name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option or joint option does not exist (Option: "
                                         + option_name + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (model->IsArray())
            {
                throw std::runtime_error("Syntax error: Option is an array (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            auto get_option_var = GetVariableFunction(option_var, linenum, filepath, manager, model);

            GetValueFunction = [&option_name, get_option_var](nemesis::CompileState& state)
            {
                auto option = state.GetBaseRequest()->GetOption(option_name);
                return option->GetVariableValue((*get_option_var)(state));
            };
            break;
        }
        case 3:
        {
            std::string& option_name  = Components.front();
            std::string& option_index = Components[1];
            std::string& option_var   = Components.back();
            auto model                = template_class->GetModel(option_name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (!model->IsArray()) throw std::runtime_error("Syntax error: Option is not an array (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");

            auto get_option
                = GetBaseOptionFunction(option_name, option_index, linenum, filepath, manager);
            auto get_option_var = GetVariableFunction(option_var, linenum, filepath, manager, model);

            GetValueFunction = [get_option, get_option_var](nemesis::CompileState& state)
            {
                auto option = (*get_option)(state);
                return option->GetVariableValue((*get_option_var)(state));
            };
            break;
        }
        case 4:
        {
            auto get_request_func    = GetTargetRequest(*template_class, manager);
            std::string& option_name = Components[2];
            std::string& option_var  = Components[3];

            auto model = template_class->GetModel(option_name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (model->IsArray()) throw std::runtime_error("Syntax error: Option is an array (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");

            auto get_option_var = GetVariableFunction(option_var, linenum, filepath, manager, model);

            GetValueFunction = [get_request_func, &option_name, get_option_var](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetOption(option_name)->GetVariableValue((*get_option_var)(state));
            };
            break;
        }
        case 5:
        {
            // how to separate base option index and non-base option index
            std::string& option_name = Components[2];
            std::string& option_index = Components[3];
            std::string& option_var  = Components[4];

            auto model = template_class->GetModel(option_name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (!model->IsArray()) throw std::runtime_error("Syntax error: Option is not array (Option: " + option_name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");

            std::string option_syntax = Components.front() + "[" + Components[1] + "][" + option_name + "]";
            auto get_request_func     = GetTargetRequest(*template_class, manager);
            auto get_option
                = GetOptionFunction(option_syntax, option_name, option_index, linenum, filepath, manager);
            auto get_option_var = GetVariableFunction(option_var, linenum, filepath, manager, model);

            GetValueFunction
                = [get_request_func, get_option, get_option_var](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                auto option = (*get_option)(request, state);
                return option->GetVariableValue((*get_option_var)(state));
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax error: Invalid option components (Expression: "
                                         + expression + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
    }
}

std::string nemesis::OptionVariableStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::OptionVariableStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

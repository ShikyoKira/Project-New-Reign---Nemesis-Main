#include "core/Statements/OptionStatement.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

#include "core/Template/TemplateClass.h"

SPtr<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*, nemesis::CompileState&)>>
nemesis::OptionStatement::BuildGetOptionFunction(const std::string& option_syntax,
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

                    throw std::runtime_error("Option must be more than " + std::to_string(i) + " in array (\""
                                             + option_name + "\")");
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
nemesis::OptionStatement::BuildGetBaseOptionFunction(const std::string& option_name,
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

        return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
            [&option_name](nemesis::CompileState& state) { return state.GetCurrentOption(option_name); });
    }

    if (!IsComplexComponent(index_str))
    {
        if (index_str == "F")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [&option_name](nemesis::CompileState& state)
                {
                    auto options = state.GetBaseRequest()->GetOptions(option_name);

                    if (!options.empty()) return options.front();

                    throw std::runtime_error("Option is required (\"" + option_name + "\")");
                });
        }

        if (index_str == "L")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
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
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
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

std::function<bool(nemesis::CompileState&)>
nemesis::OptionStatement::BuildAnyHasOptionFunction(const std::string& option_name,
                                                    const nemesis::SemanticManager& manager)
{
    auto template_class = manager.GetCurrentTemplateClass();
    auto& template_name = template_class->GetName();
    size_t num          = GetTemplateNumber(*template_class);

    if (!manager.HasRequestInQueue(Components.front())
        && !manager.HasRequestInQueue(template_name + "_" + std::to_string(num - 1)))
    {
        throw std::runtime_error("Syntax error: Unable to get target request from queue (Expression: "
                                 + Expression + ", Line: " + std::to_string(LineNum)
                                 + ", FilePath: " + FilePath.string() + ")");
    }

    if (num == 1)
    {
        return [template_name, &option_name](nemesis::CompileState& state)
        {
            auto& req_list = state.GetRequests(template_name);

            for (auto& req : req_list)
            {
                if (req->GetOption(option_name)) return true;
            }

            return false;
        };
    }

    return [this, template_name, &option_name](nemesis::CompileState& state)
    {
        auto cur_req  = state.GetCurrentRequest(Components.front());
        auto req_list = cur_req->GetParents().back()->GetRequests();

        for (auto& req : req_list)
        {
            if (req->GetOption(option_name)) return true;
        }

        return false;
    };
}

std::function<bool(nemesis::CompileState&)>
nemesis::OptionStatement::BuildAllHasOptionFunction(const std::string& option_name,
                                                    const nemesis::SemanticManager& manager)
{
    auto template_class = manager.GetCurrentTemplateClass();
    auto& template_name = template_class->GetName();
    size_t num          = GetTemplateNumber(*template_class);

    if (!manager.HasRequestInQueue(Components.front())
        && !manager.HasRequestInQueue(template_name + "_" + std::to_string(num - 1)))
    {
        throw std::runtime_error("Syntax error: Unable to get target request from queue (Expression: "
                                 + Expression + ", Line: " + std::to_string(LineNum)
                                 + ", FilePath: " + FilePath.string() + ")");
    }

    if (num == 1)
    {
        return [template_name, &option_name](nemesis::CompileState& state)
        {
            auto& req_list = state.GetRequests(template_name);

            for (auto& req : req_list)
            {
                if (!req->GetOption(option_name)) return false;
            }

            return true;
        };
    }

    return [this, template_name, &option_name](nemesis::CompileState& state)
    {
        auto cur_req  = state.GetCurrentRequest(Components.front());
        auto req_list = cur_req->GetParents().back()->GetRequests();

        for (auto& req : req_list)
        {
            if (!req->GetOption(option_name)) return false;
        }

        return true;
    };
}

nemesis::OptionStatement::OptionStatement(const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    auto template_class = manager.GetCurrentTemplateClass();

    switch (Components.size())
    {
        case 1:
        {
            std::string& name = Components.front();
            auto model        = template_class->GetModel(name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option or joint option does not exist (Option: "
                                         + name + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (model->IsArray())
            {
                throw std::runtime_error("Syntax error: Option is an array (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            GetOptionFunction
                = [&name](nemesis::CompileState& state) { return state.GetBaseRequest()->GetOption(name); };
            break;
        }
        case 2:
        {
            std::string& name  = Components.front();
            std::string& index = Components[1];
            auto model         = template_class->GetModel(name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (!model->IsArray())
            {
                throw std::runtime_error("Syntax error: Option is not an array (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            auto get_option_func = BuildGetBaseOptionFunction(name, index, linenum, filepath, manager);

            GetOptionFunction
                = [get_option_func](nemesis::CompileState& state) { return (*get_option_func)(state); };
            break;
        }
        case 3:
        {
            std::string& name     = Components[2];
            auto model = template_class->GetModel(name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (model->IsArray())
            {
                throw std::runtime_error("Syntax error: Option is an array (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            auto& req_index = Components[1];

            if (req_index == "ALL")
            {
                HasOptionFunction = BuildAllHasOptionFunction(name, manager);
                GetOptionFunction = [](nemesis::CompileState& state)
                {
                    throw std::runtime_error(
                        "Syntax error: ALL syntax cannot be used to retrieve specific option");
                    return nullptr;
                };
                return;
            }
            else if (req_index == "ANY")
            {
                HasOptionFunction = BuildAnyHasOptionFunction(name, manager);
                GetOptionFunction = [](nemesis::CompileState& state)
                {
                    throw std::runtime_error(
                        "Syntax error: ANY syntax cannot be used to retrieve specific option");
                    return nullptr;
                };
                return;
            }

            auto get_request_func = GetTargetRequest(*template_class, manager);

            GetOptionFunction = [get_request_func, &name](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetOption(name);
            };
            break;
        }
        case 4:
        {
            std::string& name  = Components[2];
            std::string& index = Components[3];
            auto model = template_class->GetModel(name);

            if (!model)
            {
                throw std::runtime_error("Syntax error: Option does not exist (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            if (!model->IsArray())
            {
                throw std::runtime_error("Syntax error: Option is not array (Option: " + name
                                         + ", Line: " + std::to_string(linenum)
                                         + ", File: " + filepath.string() + ")");
            }

            std::string option_syntax = Components.front() + "[" + Components[1] + "][" + name + "]";
            auto get_request_func     = GetTargetRequest(*template_class, manager);
            auto get_option_func
                = BuildGetOptionFunction(option_syntax, name, index, linenum, filepath, manager);

            if (!get_option_func) return;

            GetOptionFunction = [get_request_func, get_option_func](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return (*get_option_func)(request, state);
            };
            break;
        }
        default:
            throw std::runtime_error("Syntax error: Invalid option components (Expression: " + expression
                                     + ", Line: " + std::to_string(linenum) + ", File: " + filepath.string()
                                     + ")");
    }
}

std::string nemesis::OptionStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::OptionStatement::GetValue(nemesis::CompileState& state) const
{
    return std::to_string(reinterpret_cast<size_t>(this));
}

const nemesis::TemplateOption* nemesis::OptionStatement::GetOption(nemesis::CompileState& state) const
{
    return GetOptionFunction(state);
}

std::string nemesis::OptionStatement::GetVariableValue(nemesis::CompileState& state,
                                                       const std::string& variable_name) const
{
    return GetOptionFunction(state)->GetVariableValue(variable_name);
}

bool nemesis::OptionStatement::HasOption(nemesis::CompileState& state) const
{
    return HasOptionFunction(state);
}

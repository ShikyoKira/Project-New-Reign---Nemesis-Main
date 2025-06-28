#include "Core/Statement/OptionStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"

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
            ThrowSyntaxError("Option not in queue");
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

                    ThrowSyntaxError("Option is required \"" + option_name + "\"");
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

                    ThrowSyntaxError("Option is required \"" + option_name + "\"");
                });
        }

        if (is_only_number(index))
        {
            size_t i = std::stoul(index);

            return std::make_shared<std::function<const nemesis::TemplateOption*(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, i, &option_name](const nemesis::AnimationRequest* request, nemesis::CompileState& state)
                {
                    auto options = request->GetOptions(option_name);

                    if (options.size() > i) return options[i];

                    ThrowSyntaxError("Option must be more than " + std::to_string(i) + " in array");
                });
        }

        ThrowSyntaxError("Invalid option index");
    }

    auto& component = DynamicComponents.emplace_back(index, linenum, filepath, manager);
    return std::make_shared<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*,
                                                                         nemesis::CompileState&)>>(
        [this, &component, &option_name, filepath](const nemesis::AnimationRequest* request,
                                                            nemesis::CompileState& state)
        {
            std::string s_index = component.GetValue(state);

            if (s_index.empty()) return state.GetCurrentRequestOption(request, option_name);

            auto options = request->GetOptions(option_name);

            if (s_index == "F")
            {
                if (!options.empty()) return options.front();

                ThrowSyntaxError("Option is required \"" + option_name + "\"");
            }

            if (s_index == "L")
            {
                if (!options.empty()) return options.back();

                ThrowSyntaxError("Option is required \"" + option_name + "\"");
            }

            if (is_only_number(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index) return options[index];

                ThrowSyntaxError("Option must be more than " + std::to_string(index) + " in array");
            }

            ThrowSyntaxError("Invalid option index");
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
            ThrowSyntaxError("Option not in queue");
        }

        return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
            [&option_name](nemesis::CompileState& state) { return state.GetCurrentOption(option_name); });
    }

    if (!IsComplexComponent(index_str))
    {
        if (index_str == "F")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [this, &option_name](nemesis::CompileState& state)
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (!options.empty()) return options.front();

                    ThrowSyntaxError("Option is required \"" + option_name + "\"");
                });
        }

        if (index_str == "L")
        {
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [this, &option_name](nemesis::CompileState& state)
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (!options.empty()) return options.back();

                    ThrowSyntaxError("Option is required \"" + option_name + "\"");
                });
        }

        if (is_only_number(index_str))
        {
            size_t index = std::stoul(index_str);
            return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
                [this, index, &option_name](nemesis::CompileState& state)
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (options.size() > index) return options[index];

                    ThrowSyntaxError("Option must be more than " + std::to_string(index) + " in array");
                });
        }

        ThrowSyntaxError("Invalid option index");
    }

    auto& component = DynamicComponents.emplace_back(index_str, linenum, filepath, manager);
    return std::make_shared<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>(
        [this, &component, &option_name, filepath](nemesis::CompileState& state)
        {
            std::string s_index = component.GetValue(state);

            if (s_index.empty()) return state.GetCurrentOption(option_name);

            auto options = GetBaseRequest(state)->GetOptions(option_name);

            if (s_index == "F")
            {
                if (!options.empty()) return options.front();

                ThrowSyntaxError("Option is required \"" + option_name + "\"");
            }

            if (s_index == "L")
            {
                if (!options.empty()) return options.back();

                ThrowSyntaxError("Option is required \"" + option_name + "\"");
            }

            if (is_only_number(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index) return options[index];

                ThrowSyntaxError("Option must be more than " + std::to_string(index) + " in array");
            }

            ThrowSyntaxError("Invalid option index");
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
        ThrowInaccessibleError("Unable to get target request from queue");
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
        ThrowInaccessibleError("Unable to get target request from queue");
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
            const std::string& name = Components.front();
            const auto* model       = template_class->GetModel(name);

            if (!model)
            {
                ThrowSyntaxError("Option does not exist");
            }

            if (model->IsArray())
            {
                ThrowSyntaxError("Option is not array");
            }

            GetOptionFunction = [this, &name](nemesis::CompileState& state)
            { return GetBaseRequest(state)->GetOption(name); };
            break;
        }
        case 2:
        {
            const std::string& name = Components.front();
            const auto* model       = template_class->GetModel(name);

            if (!model)
            {
                ThrowSyntaxError("Option does not exist");
            }

            if (!model->IsArray())
            {
                ThrowSyntaxError("Option is not array");
            }

            const std::string& index = Components[1];
            auto get_option_func     = BuildGetBaseOptionFunction(name, index, linenum, filepath, manager);
            GetOptionFunction
                = [get_option_func](nemesis::CompileState& state) { return (*get_option_func)(state); };
            break;
        }
        case 3:
        {
            const std::string& name = Components[2];
            const auto* model       = template_class->GetModel(name);

            if (!model)
            {
                ThrowSyntaxError("Option does not exist");
            }

            if (model->IsArray())
            {
                ThrowSyntaxError("Option is not array");
            }

            const std::string& req_index = Components[1];

            if (nemesis::iequals(req_index, "ALL"))
            {
                HasOptionFunction = BuildAllHasOptionFunction(name, manager);
                GetOptionFunction = [this](nemesis::CompileState& state)
                {
                    ThrowSyntaxError("ALL syntax cannot be used to retrieve specific option");
                    return nullptr;
                };
                return;
            }
            else if (nemesis::iequals(req_index, "ANY"))
            {
                HasOptionFunction = BuildAnyHasOptionFunction(name, manager);
                GetOptionFunction = [this](nemesis::CompileState& state)
                {
                    ThrowSyntaxError("ANY syntax cannot be used to retrieve specific option");
                    return nullptr;
                };
                return;
            }

            auto get_request_func = GetTargetRequest(*template_class, manager);
            GetOptionFunction     = [get_request_func, &name](nemesis::CompileState& state)
            {
                auto request = (*get_request_func)(state);
                return request->GetOption(name);
            };
            break;
        }
        case 4:
        {
            const std::string& name = Components[2];
            const auto* model       = template_class->GetModel(name);

            if (!model)
            {
                ThrowSyntaxError("Option does not exist");
            }

            if (!model->IsArray())
            {
                ThrowSyntaxError("Option is not array");
            }

            const std::string& index        = Components.back();
            const std::string option_syntax = Components.front() + "[" + Components[1] + "][" + name + "]";
            auto get_request_func           = GetTargetRequest(*template_class, manager);
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
            ThrowSyntaxError("Invalid option components");
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

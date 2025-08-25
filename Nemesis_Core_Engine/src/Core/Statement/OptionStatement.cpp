#include "Core/Statement/OptionStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"

SPtr<std::function<nemesis::OptionStatement::OptionPairList(const nemesis::AnimationRequest*,
                                                            nemesis::CompileState&)>>
nemesis::OptionStatement::BuildGetOptionsFunction(const std::string& option_syntax,
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

        return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
            const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
            [&option_name](const nemesis::AnimationRequest* request,
                           nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair(state.GetCurrentRequestOption(request, option_name), -1)};
            });
    }

    if (!IsComplexComponent(index))
    {
        if (nemesis::iequals(index, "SIZE"))
        {
            return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [&option_name](const nemesis::AnimationRequest* request,
                               nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = request->GetOptions(option_name);
                    return nemesis::OptionStatement::OptionPairList{
                        std::make_pair<const nemesis::TemplateOption*, int>(nullptr, options.size())};
                });
        }

        if (index == "F")
        {
            return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, &option_name](const nemesis::AnimationRequest* request,
                                     nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = request->GetOptions(option_name);

                    if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options.front(), -1)};
                });
        }

        if (index == "L")
        {
            return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, &option_name](const nemesis::AnimationRequest* request,
                                     nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = request->GetOptions(option_name);

                    if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options.back(), -1)};
                });
        }

        if (is_only_number(index))
        {
            size_t i = std::stoul(index);

            return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
                const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
                [this, i, &option_name](
                    const nemesis::AnimationRequest* request,
                    nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = request->GetOptions(option_name);

                    if (options.size() > i)
                    {
                        return nemesis::OptionStatement::OptionPairList{std::make_pair(options[i], -1)};
                    }

                    ThrowSyntaxError("Option must be more than " + std::to_string(i) + " in array");
                });
        }

        ThrowSyntaxError("Invalid option index");
    }

    auto& component = DynamicComponents.emplace_back(index, linenum, filepath, manager);
    return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(
        const nemesis::AnimationRequest*, nemesis::CompileState&)>>(
        [this, &component, &option_name, filepath](
            const nemesis::AnimationRequest* request,
            nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
        {
            std::string s_index(component.GetValue(state));

            if (s_index.empty())
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair(state.GetCurrentRequestOption(request, option_name), -1)};
            }

            auto options = request->GetOptions(option_name);

            if (nemesis::iequals(s_index, "SIZE"))
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair<const nemesis::TemplateOption*, int>(nullptr, options.size())};
            }

            if (s_index == "F")
            {
                if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                return nemesis::OptionStatement::OptionPairList{std::make_pair(options.front(), -1)};
            }

            if (s_index == "L")
            {
                if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                return nemesis::OptionStatement::OptionPairList{std::make_pair(options.back(), -1)};
            }

            if (is_only_number(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index)
                {
                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options[index], -1)};
                }

                ThrowSyntaxError("Option must be more than " + std::to_string(index) + " in array");
            }

            ThrowSyntaxError("Invalid option index");
        });
}

SPtr<std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>
nemesis::OptionStatement::BuildGetBaseOptionsFunction(const std::string& option_name,
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

        return std::make_shared<
            std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
            [&option_name](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair(state.GetCurrentOption(option_name), -1)};
            });
    }

    if (!IsComplexComponent(index_str))
    {
        if (nemesis::iequals(index_str, "SIZE"))
        {
            return std::make_shared<
                std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
                [this, &option_name](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);
                    return nemesis::OptionStatement::OptionPairList{
                        std::make_pair<const nemesis::TemplateOption*, int>(nullptr, options.size())};
                });
        }

        if (index_str == "F")
        {
            return std::make_shared<
                std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
                [this, &option_name](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options.front(), -1)};
                });
        }

        if (index_str == "L")
        {
            return std::make_shared<
                std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
                [this, &option_name](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options.back(), -1)};
                });
        }

        if (is_only_number(index_str))
        {
            size_t index = std::stoul(index_str);
            return std::make_shared<
                std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
                [this, index, &option_name](
                    nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    auto options = GetBaseRequest(state)->GetOptions(option_name);

                    if (options.size() > index)
                    {
                        return nemesis::OptionStatement::OptionPairList{std::make_pair(options[index], -1)};
                    }

                    ThrowSyntaxError("Option must be more than " + std::to_string(index) + " in array");
                });
        }

        ThrowSyntaxError("Invalid option index");
    }

    auto& component = DynamicComponents.emplace_back(index_str, linenum, filepath, manager);
    return std::make_shared<std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>(
        [this, &component, &option_name, filepath](
            nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
        {
            std::string s_index(component.GetValue(state));

            if (s_index.empty())
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair(state.GetCurrentOption(option_name), -1)};
            }

            auto options = GetBaseRequest(state)->GetOptions(option_name);

            if (nemesis::iequals(s_index, "SIZE"))
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair<const nemesis::TemplateOption*, int>(nullptr, options.size())};
            }

            if (s_index == "F")
            {
                if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                return nemesis::OptionStatement::OptionPairList{std::make_pair(options.front(), -1)};
            }

            if (s_index == "L")
            {
                if (options.empty()) ThrowSyntaxError("Option is required \"" + option_name + "\"");

                return nemesis::OptionStatement::OptionPairList{std::make_pair(options.back(), -1)};
            }

            if (is_only_number(s_index))
            {
                size_t index = std::stoi(s_index);

                if (options.size() > index)
                {
                    return nemesis::OptionStatement::OptionPairList{std::make_pair(options[index], -1)};
                }

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
        auto& cur_req = state.GetCurrentRequest(Components.front());
        auto req_list = cur_req.GetParents().back()->GetRequests();

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
        auto& cur_req = state.GetCurrentRequest(Components.front());
        auto req_list = cur_req.GetParents().back()->GetRequests();

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
        case 1: // T
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

            GetOptionsFunction
                = [this, &name](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            {
                return nemesis::OptionStatement::OptionPairList{
                    std::make_pair(GetBaseRequest(state)->GetOption(name), -1)};
            };
            break;
        }
        case 2: // T[]
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
            auto get_options_func    = BuildGetBaseOptionsFunction(name, index, linenum, filepath, manager);
            GetOptionsFunction
                = [get_options_func](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            { return (*get_options_func)(state); };
            break;
        }
        case 3: // s_2[][T]
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
                HasOptionFunction  = BuildAllHasOptionFunction(name, manager);
                GetOptionsFunction
                    = [this](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    ThrowSyntaxError("ALL syntax cannot be used to retrieve specific option");
                    return nemesis::OptionStatement::OptionPairList{
                        std::make_pair<const nemesis::TemplateOption*, int>(nullptr, -1)};
                };
                return;
            }
            else if (nemesis::iequals(req_index, "ANY"))
            {
                HasOptionFunction  = BuildAnyHasOptionFunction(name, manager);
                GetOptionsFunction
                    = [this](nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
                {
                    ThrowSyntaxError("ANY syntax cannot be used to retrieve specific option");
                    return nemesis::OptionStatement::OptionPairList();
                };
                return;
            }

            auto get_request_func = GetTargetRequest(*template_class, manager);
            GetOptionsFunction    = [get_request_func, &name](
                                     nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            {
                auto& request = (*get_request_func)(state);
                return nemesis::OptionStatement::OptionPairList{std::make_pair(request.GetOption(name), -1)};
            };
            break;
        }
        case 4: // s_2[][T][event]
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
            auto get_property_accessor      = GetTargetAggregatePropertyAccessor(*template_class, manager);
            auto get_options_func
                = BuildGetOptionsFunction(option_syntax, name, index, linenum, filepath, manager);

            if (!get_options_func) return;

            GetOptionsFunction = [get_property_accessor, get_options_func](
                                     nemesis::CompileState& state) -> nemesis::OptionStatement::OptionPairList
            {
                nemesis::OptionStatement::OptionPairList aggr_pair_list;
                (*get_property_accessor)(
                    [&aggr_pair_list, get_options_func](const nemesis::AnimationRequest& request,
                                              nemesis::CompileState& state) -> std::string
                    {
                        auto pair_list = (*get_options_func)(&request, state);
                        aggr_pair_list.insert(aggr_pair_list.end(), pair_list.begin(), pair_list.end());
                        return "";
                    },
                    state);
                return aggr_pair_list;
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

Pair<const nemesis::TemplateOption*, int>
nemesis::OptionStatement::GetValue(nemesis::CompileState& state) const
{
    return GetOptionsFunction(state).front();
}

Vec<Pair<const nemesis::TemplateOption*, int>>
nemesis::OptionStatement::GetValues(nemesis::CompileState& state) const
{
    return GetOptionsFunction(state);
}

int nemesis::OptionStatement::GetSize(nemesis::CompileState& state) const
{
    return GetOptionsFunction(state).front().second;
}

const nemesis::TemplateOption* nemesis::OptionStatement::GetOption(nemesis::CompileState& state) const
{
    return GetOptionsFunction(state).front().first;
}

std::string nemesis::OptionStatement::GetVariableValue(nemesis::CompileState& state,
                                                       const std::string& variable_name) const
{
    auto* option = GetOptionsFunction(state).front().first;

    if (!option) ThrowInaccessibleError("Failed to access required option");

    return option->GetVariableValue(variable_name);
}

bool nemesis::OptionStatement::HasOption(nemesis::CompileState& state) const
{
    return HasOptionFunction(state);
}

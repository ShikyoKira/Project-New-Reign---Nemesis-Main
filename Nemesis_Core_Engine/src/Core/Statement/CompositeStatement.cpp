#include <regex>

#include "Core/Statement/CompositeStatement.h"

#include "Core/LineModifier/LineModifier.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

const nemesis::AnimationRequest*
nemesis::CompositeStatement::GetAnimationRequest(const std::string& index_str,
                                                 const std::string& templt_name,
                                                 size_t templt_num,
                                                 nemesis::CompileState& state,
                                                 nemesis::SemanticManager& manager) const
{
    const std::string& templt_code = Components.front();

    if (!std::regex_match(templt_code, std::regex("^" + templt_name + "_[0-9]+$")))
    {
        ThrowTemplateUnsupported("Template unsupported '" + templt_code + "'", templt_name);
    }

    if (is_only_number(index_str))
    {
        if (!manager.HasRequestInQueue(templt_code)
            && !manager.HasRequestInQueue(templt_name + "_" + std::to_string(templt_num - 1)))
        {
            ThrowInaccessibleError("Unable to get target request from queue");
        }

        size_t index = std::stoul(index_str);
        auto request = state.GetCurrentRequest(templt_code);
        auto parents = request->GetParents();

        if (!parents.empty())
        {
            auto list = parents.back()->GetRequests();

            if (index < list.size()) return list[index];

            ThrowInvalidError("Index is larger than list");
        }

        {
            auto& collection = state.GetRequests(request->GetTemplateName());

            if (index < collection.size()) return collection[index];
        }

        ThrowInvalidError("Index is larger than list");
    }

    if (index_str.size() > 1)
    {
        ThrowInvalidError("Invalid index value (" + index_str + ")");
    }

    if (index_str == "")
    {
        if (!manager.HasRequestInQueue(templt_code)
            && !manager.HasRequestInQueue(templt_name + "_" + std::to_string(templt_num - 1)))
        {
            ThrowInaccessibleError("Unable to get target request from queue");
        }

        return state.GetCurrentRequest(templt_code);
    }

    switch (index_str.front())
    {
        case 'F':
        {
            if (!manager.HasRequestInQueue(templt_code)
                && !manager.HasRequestInQueue(templt_name + "_" + std::to_string(templt_num - 1)))
            {
                ThrowInaccessibleError("Unable to get target request from queue");
            }

            return state.GetFirstRequest(templt_code);
        }
        case 'L':
        {
            if (!manager.HasRequestInQueue(templt_code)
                && !manager.HasRequestInQueue(templt_name + "_" + std::to_string(templt_num - 1)))
            {
                ThrowInaccessibleError("Unable to get target request from queue");
            }

            return state.GetLastRequest(templt_code);
        }
        case 'B':
        {
            if (!manager.HasRequestInQueue(templt_code))
            {
                ThrowInaccessibleError("Unable to get target request from queue");
            }

            return state.GetBackRequest(templt_code);
        }
        case 'N':
        {
            if (!manager.HasRequestInQueue(templt_code))
            {
                ThrowInaccessibleError("Unable to get target request from queue");
            }

            return state.GetNextRequest(templt_code);
        }
        default:
            ThrowSyntaxError("Invalid value (" + index_str + ")");
    }
}

nemesis::CompositeStatement::DynamicComponent::DynamicComponent(const std::string& component,
                                                                size_t linenum,
                                                                const std::filesystem::path& filepath,
                                                                const nemesis::SemanticManager& manager)
    : RawValue(component)
{
    int layer = 0;
    Vec<Pair<size_t, size_t>> pos_list;

    for (size_t i = 0; i < component.length(); ++i)
    {
        const char& ch = component[i];
        Tokens.emplace_back(1, ch);

        switch (ch)
        {
            case '{':
            {
                if (layer++ != 0) break;

                pos_list.emplace_back(i + 1, 0);
                break;
            }
            case '}':
            {
                if (--layer < 0)
                {
                    throw std::runtime_error("Syntax Error: Unopened '}' (Component: " + component
                                             + ", Line: " + std::to_string(linenum)
                                             + ", File: " + nemesis::to_utf8_string(filepath) + ")");
                }

                if (layer > 0) break;

                pos_list.back() = std::make_pair(pos_list.back().first, i + 1);
                break;
            }
        }
    }

    if (pos_list.empty()) return;

    for (auto& pos_pair : pos_list)
    {
        std::string cur  = component.substr(pos_pair.first, pos_pair.second - pos_pair.first - 1);
        auto sh_modifier = nemesis::LineModifierFactory::BuildModifier(
            pos_pair.first, pos_pair.second, cur, component, linenum, filepath, manager);
        Modifiers.emplace_back(sh_modifier);
    }
}

std::string nemesis::CompositeStatement::DynamicComponent::GetValue(nemesis::CompileState& state) const
{
    if (Modifiers.empty()) return RawValue;

    VecStr tokens(Tokens);
    std::string merged;

    for (auto& modifier : Modifiers)
    {
        modifier->Apply(tokens, state);
    }

    for (auto& token : tokens)
    {
        merged.append(token);
    }

    return merged;
}

bool nemesis::CompositeStatement::DynamicComponent::IsDynamic() const
{
    return !Modifiers.empty();
}

bool nemesis::CompositeStatement::IsComplexComponent(const std::string& component)
{
    size_t open  = 0;
    size_t close = 0;

    for (const char& ch : component)
    {
        switch (ch)
        {
            case '{':
                ++open;
                break;
            case '}':
                ++close;
                break;
            default:
                break;
        }
    }

    return open > 0 && open == close;
}

SPtr<std::function<bool(nemesis::CompileState&)>> nemesis::CompositeStatement::CallbackTargetRequests(
    const nemesis::TemplateClass& templt_class,
    const nemesis::SemanticManager& manager,
    const std::function<bool(nemesis::CompileState&, const nemesis::AnimationRequest*)>& callback)
{
    const std::string& index_str = Components[1];

    if (!IsComplexComponent(index_str))
    {
        return nemesis::Statement::CallbackTargetRequests(templt_class, manager, callback);
    }

    auto& templt_name = templt_class.GetName();
    auto& templt_code = Components.front();

    if (!std::regex_match(templt_code, std::regex("^" + templt_name + "_[0-9]+$")))
    {
        ThrowTemplateUnsupported("Template unsupported '" + templt_code + "'", templt_name);
    }

    size_t templt_num         = GetTemplateNumber(templt_class);
    const auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
    SPtr<nemesis::SemanticManager> sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);
    return std::make_shared<std::function<bool(nemesis::CompileState&)>>(
        [this, &dynamic_index, sptr_manager, &templt_code, &templt_name, templt_num, callback](
            nemesis::CompileState& state)
        {
            const std::string index_str = dynamic_index.GetValue(state);

            if (!nemesis::iequals(index_str, "ANY") && !nemesis::iequals(index_str, "ALL"))
            {
                auto request = GetAnimationRequest(index_str, templt_name, templt_num, state, *sptr_manager);
                return callback(state, request);
            }

            if (nemesis::iequals(index_str, "ANY"))
            {
                auto request = state.GetCurrentRequest(templt_code);
                auto parents = request->GetParents();

                // Vacuous false: Default to false
                if (!parents.empty())
                {
                    for (auto& request : parents.back()->GetRequests())
                    {
                        if (!callback(state, request)) continue;

                        return true;
                    }

                    return false;
                }

                for (auto& request : state.GetRequests(request->GetTemplateName()))
                {
                    if (!callback(state, request)) continue;

                    return true;
                }

                return false;
            }

            auto request = state.GetCurrentRequest(templt_code);
            auto parents = request->GetParents();

            // Vacuous false: Default to false
            if (!parents.empty())
            {
                auto req_list = parents.back()->GetRequests();

                if (req_list.empty()) return false;

                for (auto& request : req_list)
                {
                    if (callback(state, request)) continue;

                    return false;
                }

                return true;
            }

            auto& collection = state.GetRequests(request->GetTemplateName());

            if (collection.empty()) return false;

            for (auto& request : collection)
            {
                if (callback(state, request)) continue;

                return false;
            }

            return true;
        });
}

SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>
nemesis::CompositeStatement::GetTargetRequest(const nemesis::TemplateClass& templt_class,
                                              const nemesis::SemanticManager& manager)
{
    const std::string& index_str = Components[1];

    if (!IsComplexComponent(index_str)) return nemesis::Statement::GetTargetRequest(templt_class, manager);

    size_t num        = GetTemplateNumber(templt_class);
    auto& templt_name = templt_class.GetName();

    if (!std::regex_match(Components.front(), std::regex("^" + templt_name + "_[0-9]+$")))
    {
        ThrowTemplateUnsupported("Template unsupported '" + Components.front() + "'", templt_name);
    }

    const auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
    SPtr<nemesis::SemanticManager> sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);
    return std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
        [this, &dynamic_index, sptr_manager, templt_name, num](nemesis::CompileState& state)
        {
            const std::string index_str = dynamic_index.GetValue(state);
            return GetAnimationRequest(index_str, templt_name, num, state, *sptr_manager);
        });
}

nemesis::CompositeStatement::CompositeStatement(const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath)
    : nemesis::Statement(expression, linenum, filepath)
{
}

std::string nemesis::CompositeStatement::Serialize() const
{
    return Expression;
}

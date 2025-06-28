#include "Core/Statement/MapStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

bool nemesis::MapStatement::TryParse3Components(const nemesis::SemanticManager& manager)
{
    const std::string& key       = Components[1];
    const std::string& index_str = Components.back();

    SPtr<std::function<std::string(nemesis::CompileState&)>> get_key;

    if (IsComplexComponent(key))
    {
        auto& dynamic_key = DynamicComponents.emplace_back(key, LineNum, FilePath, manager);
        get_key           = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_key](nemesis::CompileState& state) { return dynamic_key.GetValue(state); });
    }
    else
    {
        get_key = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&key](nemesis::CompileState& state) { return key; });
    }

    if (IsComplexComponent(index_str))
    {
        const auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
        auto get_index            = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_index](nemesis::CompileState& state) { return dynamic_index.GetValue(state); });
        auto sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);
        GetValueFunction  = [this, get_key, get_index, sptr_manager](nemesis::CompileState& state)
        {
            auto key  = (*get_key)(state);
            auto list = GetBaseRequest(state)->GetMapValueList(key);

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return *list[index];

                ThrowInvalidError("Index is larger than list");
            }

            if (index_str.length() > 1)
            {
                ThrowInvalidError("Invalid index value (" + index_str + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasMapInQueue(Components[1]))
                {
                    ThrowInaccessibleError("Unable to get target map value from queue");
                }

                return state.GetCurrentMapValue(key);
            }

            switch (index_str.front())
            {
                case 'F':
                {
                    return *list.front();
                }
                case 'L':
                {
                    return *list.back();
                }
                case 'B':
                {
                    if (!sptr_manager->HasMapInQueue(Components[1]))
                    {
                        ThrowInaccessibleError("Unable to get target map value from queue");
                    }

                    size_t index = state.GetCurrentMapIndex(key);

                    if (index == 0) return *list.front();

                    return *list[index - 1];
                }
                case 'N':
                {
                    if (!sptr_manager->HasMapInQueue(Components[1]))
                    {
                        ThrowInaccessibleError("Unable to get target map value from queue");
                    }

                    size_t index = state.GetCurrentMapIndex(key);

                    if (index + 1 == list.size()) return *list.back();

                    return *list[index + 1];
                }
                default:
                    ThrowSyntaxError("Unsupported Map components");
            }
        };
        return true;
    }

    if (is_only_number(index_str))
    {
        size_t index     = std::stoul(index_str);
        GetValueFunction = [this, get_key, index](nemesis::CompileState& state)
        {
            auto list = GetBaseRequest(state)->GetMapValueList((*get_key)(state));

            if (list.empty()) return std::string("");

            if (index < list.size()) return *list[index];

            ThrowInvalidError("Index is larger than list");
        };
        return true;
    }

    if (index_str.length() > 1) return false;

    if (index_str == "")
    {
        GetValueFunction
            = [get_key](nemesis::CompileState& state) { return state.GetCurrentMapValue((*get_key)(state)); };
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                GetValueFunction = [this, get_key](nemesis::CompileState& state)
                {
                    auto list = GetBaseRequest(state)->GetMapValueList((*get_key)(state));

                    if (list.empty()) return std::string("");

                    return *list.front();
                };
                return true;
            }
            case 'L':
            {
                GetValueFunction = [get_key](nemesis::CompileState& state)
                { return state.GetCurrentMapValue((*get_key)(state)); };
                return true;
            }
            case 'B':
            {
                GetValueFunction = [this, get_key](nemesis::CompileState& state)
                {
                    auto key   = (*get_key)(state);
                    auto list  = GetBaseRequest(state)->GetMapValueList(key);
                    auto index = state.GetCurrentMapIndex(key);

                    if (index == 0) return *list.front();

                    return *list[index - 1];
                };
                break;
            }
            case 'N':
            {
                GetValueFunction = [this, get_key](nemesis::CompileState& state)
                {
                    auto key   = (*get_key)(state);
                    auto list  = GetBaseRequest(state)->GetMapValueList(key);
                    auto index = state.GetCurrentMapIndex(key);

                    if (index == list.size() - 1) return *list.back();

                    return *list[index + 1];
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasMapInQueue(key)) return true;

    ThrowInaccessibleError("Unable to get target map value from queue");
}

bool nemesis::MapStatement::TryParse5Components(const nemesis::SemanticManager& manager)
{
    const std::string& key       = Components[3];
    const std::string& index_str = Components.back();
    auto template_class          = manager.GetCurrentTemplateClass();
    auto get_request             = GetTargetRequest(*template_class, manager);

    SPtr<std::function<std::string(nemesis::CompileState&)>> get_key;

    if (IsComplexComponent(key))
    {
        auto& dynamic_key = DynamicComponents.emplace_back(key, LineNum, FilePath, manager);
        get_key           = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_key](nemesis::CompileState& state) { return dynamic_key.GetValue(state); });
    }
    else
    {
        get_key = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&key](nemesis::CompileState& state) { return key; });
    }

    if (IsComplexComponent(index_str))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
        auto get_index      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_index](nemesis::CompileState& state) { return dynamic_index.GetValue(state); });
        SPtr<nemesis::SemanticManager> sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);

        GetValueFunction = [this, get_request, get_key, get_index, sptr_manager](nemesis::CompileState& state)
        {
            auto key     = (*get_key)(state);
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return *list[index];

                ThrowInvalidError("Index is larger than list");
            }

            if (index_str.length() > 1)
            {
                ThrowInvalidError("Invalid index value (" + index_str + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasRequestMapInQueue(Components.front(), key))
                {
                    ThrowInaccessibleError("Unable to get target map value from queue");
                }

                return state.GetCurrentRequestMapValue((*get_request)(state), key);
            }

            switch (index_str.front())
            {
                case 'F':
                {
                    return *list.front();
                }
                case 'L':
                {
                    return *list.back();
                }
                case 'B':
                {
                    if (!sptr_manager->HasRequestMapInQueue(Components.front(), key))
                    {
                        ThrowInaccessibleError("Unable to get target map value from queue");
                    }

                    auto index = state.GetCurrentRequestMapIndex(request, key);

                    if (index == 0) return *list.front();

                    return *list[index - 1];
                }
                case 'N':
                {
                    if (!sptr_manager->HasRequestMapInQueue(Components.front(), key))
                    {
                        ThrowInaccessibleError("Unable to get target map value from queue");
                    }

                    auto index = state.GetCurrentRequestMapIndex(request, key);

                    if (index + 1 == list.size()) return *list.back();

                    return *list[index + 1];
                }
                default:
                    ThrowSyntaxError("Unsupported Map components");
            }
        };
        return true;
    }

    if (is_only_number(index_str))
    {
        size_t index     = std::stoul(index_str);
        GetValueFunction = [this, get_key, get_request, index](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList((*get_key)(state));

            if (list.empty()) return std::string("");

            if (index < list.size()) return *list[index];

            ThrowInvalidError("Index is larger than list");
        };
        return true;
    }

    if (index_str.length() > 1) return false;

    if (index_str == "")
    {
        GetValueFunction = [get_key, get_request](nemesis::CompileState& state)
        { return state.GetCurrentRequestMapValue((*get_request)(state), (*get_key)(state)); };
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                GetValueFunction = [get_key, get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMapValueList((*get_key)(state));

                    if (list.empty()) return std::string("");

                    return *list.front();
                };
                return true;
            }
            case 'L':
            {
                GetValueFunction = [get_key, get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMapValueList((*get_key)(state));

                    if (list.empty()) return std::string("");

                    return *list.back();
                };
                return true;
            }
            case 'B':
            {
                GetValueFunction = [get_key, get_request](nemesis::CompileState& state)
                {
                    auto key     = (*get_key)(state);
                    auto request = (*get_request)(state);
                    auto list    = request->GetMapValueList(key);
                    size_t index = state.GetCurrentRequestMapIndex(request, key);

                    if (index == 0) return *list.front();

                    return *list[index - 1];
                };
                break;
            }
            case 'N':
            {
                GetValueFunction = [get_key, get_request](nemesis::CompileState& state)
                {
                    auto key     = (*get_key)(state);
                    auto request = (*get_request)(state);
                    auto list    = request->GetMapValueList(key);
                    size_t index = state.GetCurrentRequestMapIndex(request, key);

                    if (index + 1 == list.size()) return *list.back();

                    return *list[index + 1];
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasRequestMapInQueue(Components.front(), key)) return true;

    ThrowInaccessibleError("Unable to get target map value from queue");
}

nemesis::MapStatement::MapStatement(const std::string& expression,
                                    size_t linenum,
                                    const std::filesystem::path& filepath,
                                    const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 3:
        {
            if (TryParse3Components(manager)) return;

            break;
        }
        case 5:
        {
            if (TryParse5Components(manager)) return;

            break;
        }
        default:
            break;
    }

    ThrowSyntaxError("Unsupported Map components");
}

std::string nemesis::MapStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

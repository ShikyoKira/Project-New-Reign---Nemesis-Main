#include "Core/Statement/MotionDataStatement.h"
#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

bool nemesis::MotionDataStatement::TryParse2Components(const nemesis::SemanticManager& manager)
{
    const std::string& index_str = Components.back();

    if (IsComplexComponent(index_str))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
        auto get_index      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_index](nemesis::CompileState& state) { return dynamic_index.GetValue(state); });
        auto sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);
        GetValueFunction  = [this, get_index, sptr_manager](nemesis::CompileState& state)
        {
            auto list = GetBaseRequest(state)->GetMotionDataList();

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return list[index].ToString();

                ThrowInvalidError("Index is larger than list");
            }

            if (index_str.length() > 1)
            {
                ThrowInvalidError("Invalid index value (" + index_str + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasMotionDataInQueue())
                {
                    ThrowInaccessibleError("Unable to get target motion data from queue");
                }

                return state.GetCurrentMotionData();
            }

            switch (index_str.front())
            {
                case 'F':
                {
                    return list.front().ToString();
                }
                case 'L':
                {
                    return list.back().ToString();
                }
                case 'B':
                {
                    if (!sptr_manager->HasMotionDataInQueue())
                    {
                        ThrowInaccessibleError("Unable to get target motion data from queue");
                    }

                    size_t index = state.GetCurrentMotionIndex();

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                }
                case 'N':
                {
                    if (!sptr_manager->HasMotionDataInQueue())
                    {
                        ThrowInaccessibleError("Unable to get target motion data from queue");
                    }

                    size_t index = state.GetCurrentMotionIndex();

                    if (index + 1 == list.size()) return list.front().ToString();

                    return list[index + 1].ToString();
                }
                default:
                    ThrowSyntaxError("Unsupported Motion Data components");
            }
        };
        return true;
    }

    if (is_only_number(index_str))
    {
        size_t index     = std::stoul(index_str);
        GetValueFunction = [this, index](nemesis::CompileState& state)
        {
            auto list = GetBaseRequest(state)->GetMotionDataList();

            if (index < list.size()) return list[index].ToString();

            ThrowInvalidError("Index is larger than list");
        };
        return true;
    }

    if (index_str.length() > 1) return false;

    if (index_str == "")
    {
        GetValueFunction = [](nemesis::CompileState& state) { return state.GetCurrentMotionData(); };
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list = GetBaseRequest(state)->GetMotionDataList();

                    if (list.empty()) return std::string("");

                    return list.front().ToString();
                };
                return true;
            }
            case 'L':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list = GetBaseRequest(state)->GetMotionDataList();

                    if (list.empty()) return std::string("");

                    return list.back().ToString();
                };
                return true;
            }
            case 'B':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list    = GetBaseRequest(state)->GetMotionDataList();
                    size_t index = state.GetCurrentMotionIndex();

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                };
                break;
            }
            case 'N':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto request = GetBaseRequest(state);
                    auto list    = request->GetMotionDataList();
                    size_t index = state.GetCurrentMotionIndex();

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasMotionDataInQueue()) return true;

    ThrowInaccessibleError("Unable to get target motion data from queue");
}

bool nemesis::MotionDataStatement::TryParse4Components(const nemesis::SemanticManager& manager)
{
    const std::string& index_str = Components.back();
    auto template_class          = manager.GetCurrentTemplateClass();
    auto get_request             = GetTargetRequest(*template_class, manager);

    if (IsComplexComponent(index_str))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index_str, LineNum, FilePath, manager);
        auto get_index      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_index](nemesis::CompileState& state) { return dynamic_index.GetValue(state); });
        auto sptr_manager = std::make_shared<nemesis::SemanticManager>(manager);
        GetValueFunction  = [this, get_request, get_index, sptr_manager](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMotionDataList();

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return list[index].ToString();

                ThrowInvalidError("Index is larger than list");
            }

            if (index_str.length() > 1)
            {
                ThrowInvalidError("Invalid index value (" + index_str + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasRequestMotionDataInQueue(Components.front()))
                {
                    ThrowInaccessibleError("Unable to get target motion data from queue");
                }

                return state.GetCurrentRequestMotionData(request);
            }

            switch (index_str.front())
            {
                case 'F':
                {
                    return list.front().ToString();
                }
                case 'L':
                {
                    return list.back().ToString();
                }
                case 'B':
                {
                    if (!sptr_manager->HasRequestMotionDataInQueue(Components.front()))
                    {
                        ThrowInaccessibleError("Unable to get target motion data from queue");
                    }

                    size_t index = state.GetCurrentRequestMotionIndex(request);

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                }
                case 'N':
                {
                    if (!sptr_manager->HasRequestMotionDataInQueue(Components.front()))
                    {
                        ThrowInaccessibleError("Unable to get target motion data from queue");
                    }

                    size_t index = state.GetCurrentRequestMotionIndex(request);

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                }
                default:
                    ThrowSyntaxError("Unsupported Motion Data components");
            }
        };
        return true;
    }

    if (is_only_number(index_str))
    {
        size_t index     = std::stoul(index_str);
        GetValueFunction = [get_request, index](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMotionDataList();

            if (list.empty()) return std::string("");

            return list[index].ToString();
        };
        return true;
    }

    if (index_str.length() > 1) return false;

    if (index_str == "")
    {
        GetValueFunction = [get_request](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            return state.GetCurrentRequestMotionData(request);
        };
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                GetValueFunction = [get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMotionDataList();

                    if (list.empty()) return std::string("");

                    return list.front().ToString();
                };
                return true;
            }
            case 'L':
            {
                GetValueFunction = [get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMotionDataList();

                    if (list.empty()) return std::string("");

                    return list.back().ToString();
                };
                return true;
            }
            case 'B':
            {
                GetValueFunction = [get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMotionDataList();
                    size_t index = state.GetCurrentRequestMotionIndex(request);

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                };
                break;
            }
            case 'N':
            {
                GetValueFunction = [get_request](nemesis::CompileState& state)
                {
                    auto request = (*get_request)(state);
                    auto list    = request->GetMotionDataList();
                    size_t index = state.GetCurrentRequestMotionIndex(request);

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasRequestMotionDataInQueue(Components.front())) return true;

    ThrowInaccessibleError("Unable to get target motion data from queue");
}

nemesis::MotionDataStatement::MotionDataStatement(const std::string& expression,
                                                  size_t linenum,
                                                  const std::filesystem::path& filepath,
                                                  const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 2:
        {
            if (TryParse2Components(manager)) return;

            break;
        }
        case 4:
        {
            if (TryParse4Components(manager)) return;

            break;
        }
        default:
            break;
    }

    ThrowSyntaxError("Unsupported Motion Data components");
}

std::string nemesis::MotionDataStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

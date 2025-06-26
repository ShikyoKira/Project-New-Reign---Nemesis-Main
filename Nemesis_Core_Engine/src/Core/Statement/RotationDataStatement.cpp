#include "Core/Statement/RotationDataStatement.h"
#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"

bool nemesis::RotationDataStatement::TryParse2Components(const nemesis::SemanticManager& manager)
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
            auto list = GetBaseRequest(state)->GetRotationDataList();

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return list[index].ToString();

                throw std::runtime_error("Value Unaccessible: Index is larger than list (Syntax: "
                                         + Expression + ", Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            if (index_str.length() > 1)
            {
            InvalidIndex:
                throw std::runtime_error("Invalid index (" + index_str + ") value ("
                                         + std::to_string(list.size()) + ") (Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasRotationDataInQueue())
                {
                SyntaxError:
                    throw std::runtime_error(
                        "Syntax Error: Unable to get target rotation data from queue (Syntax: " + Expression
                        + ", Line: " + std::to_string(LineNum)
                        + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
                }

                return state.GetCurrentRotationData();
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
                    if (!sptr_manager->HasRotationDataInQueue())
                    {
                        goto SyntaxError;
                    }

                    size_t index = state.GetCurrentRotationIndex();

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                }
                case 'N':
                {
                    if (!sptr_manager->HasRotationDataInQueue())
                    {
                        goto SyntaxError;
                    }

                    size_t index = state.GetCurrentRotationIndex();

                    if (index + 1 == list.size()) return list.front().ToString();

                    return list[index + 1].ToString();
                }
                default:
                    goto InvalidIndex;
            }
        };
        return true;
    }

    if (is_only_number(index_str))
    {
        size_t index     = std::stoul(index_str);
        GetValueFunction = [this, index](nemesis::CompileState& state)
        {
            auto list = GetBaseRequest(state)->GetRotationDataList();

            if (index < list.size()) return list[index].ToString();

            throw std::runtime_error("Value Unaccessible: Index is larger than list (Syntax: " + Expression
                                     + ", Line: " + std::to_string(LineNum)
                                     + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
        };
        return true;
    }

    if (index_str.length() > 1) return false;

    if (index_str == "")
    {
        GetValueFunction = [](nemesis::CompileState& state) { return state.GetCurrentRotationData(); };
    }
    else
    {
        switch (index_str.front())
        {
            case 'F':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list = GetBaseRequest(state)->GetRotationDataList();

                    if (list.empty()) return std::string("");

                    return list.front().ToString();
                };
                return true;
            }
            case 'L':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list = GetBaseRequest(state)->GetRotationDataList();

                    if (list.empty()) return std::string("");

                    return list.back().ToString();
                };
                return true;
            }
            case 'B':
            {
                GetValueFunction = [this](nemesis::CompileState& state)
                {
                    auto list    = GetBaseRequest(state)->GetRotationDataList();
                    size_t index = state.GetCurrentRotationIndex();

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
                    auto list    = request->GetRotationDataList();
                    size_t index = state.GetCurrentRotationIndex();

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasRotationDataInQueue()) return true;

    throw std::runtime_error("Syntax Error: Unable to get target rotation data from queue (Syntax: "
                             + Expression + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

bool nemesis::RotationDataStatement::TryParse4Components(const nemesis::SemanticManager& manager)
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
            auto list    = request->GetRotationDataList();

            if (list.empty()) return std::string("");

            std::string index_str = (*get_index)(state);

            if (is_only_number(index_str))
            {
                size_t index = std::stoul(index_str);

                if (index < list.size()) return list[index].ToString();

                throw std::runtime_error("Value Unaccessible: Index is larger than list (Syntax: "
                                         + Expression + ", Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            if (index_str.length() > 1)
            {
            InvalidIndex:
                throw std::runtime_error("Invalid index (" + index_str + ") value ("
                                         + std::to_string(list.size()) + ") (Line: " + std::to_string(LineNum)
                                         + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
            }

            if (index_str == "")
            {
                if (!sptr_manager->HasRequestRotationDataInQueue(Components.front()))
                {
                SyntaxError:
                    throw std::runtime_error(
                        "Syntax Error: Unable to get target rotation data from queue (Syntax: " + Expression
                        + ", Line: " + std::to_string(LineNum)
                        + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
                }

                return state.GetCurrentRequestRotationData(request);
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
                    if (!sptr_manager->HasRequestRotationDataInQueue(Components.front()))
                    {
                        goto SyntaxError;
                    }

                    size_t index = state.GetCurrentRequestRotationIndex(request);

                    if (index == 0) return list.front().ToString();

                    return list[index - 1].ToString();
                }
                case 'N':
                {
                    if (!sptr_manager->HasRequestRotationDataInQueue(Components.front()))
                    {
                        goto SyntaxError;
                    }

                    size_t index = state.GetCurrentRequestRotationIndex(request);

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                }
                default:
                    goto InvalidIndex;
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
            auto list    = request->GetRotationDataList();

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
            return state.GetCurrentRequestRotationData(request);
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
                    auto list    = request->GetRotationDataList();

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
                    auto list    = request->GetRotationDataList();

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
                    auto list    = request->GetRotationDataList();
                    size_t index = state.GetCurrentRequestRotationIndex(request);

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
                    auto list    = request->GetRotationDataList();
                    size_t index = state.GetCurrentRequestRotationIndex(request);

                    if (index + 1 == list.size()) return list.back().ToString();

                    return list[index + 1].ToString();
                };
                break;
            }
            default:
                return false;
        }
    }

    if (manager.HasRequestRotationDataInQueue(Components.front())) return true;

    throw std::runtime_error("Syntax Error: Unable to get target rotation data from queue (Syntax: "
                             + Expression + ", Line: " + std::to_string(LineNum)
                             + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
}

nemesis::RotationDataStatement::RotationDataStatement(const std::string& expression,
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

    throw std::runtime_error("Syntax Error: Unsupported Rotation Data components (Expression: " + expression
                             + ", Line: " + std::to_string(linenum)
                             + ", File: " + nemesis::to_utf8_string(filepath) + ")");
}

std::string nemesis::RotationDataStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

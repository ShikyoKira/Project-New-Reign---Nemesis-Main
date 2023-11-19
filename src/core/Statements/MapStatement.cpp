#include "core/Statements/MapStatement.h"

#include "core/CompileState.h"
#include "core/SemanticManager.h"

bool nemesis::MapStatement::SetupGetValueFunction(
    SPtr<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>> get_request,
    const std::string& key,
    const std::string& index_str,
    const nemesis::SemanticManager& manager)
{
    if (index_str == "")
    {
        GetValueFunction = [&key, get_request](nemesis::CompileState& state)
        {
            auto map_val = state.GetCurrentRequestMapValue((*get_request)(state), key);

            if (!map_val) return std::string("");

            return *map_val;
        };
        return true;
    }
    else if (index_str == "F")
    {
        GetValueFunction = [&key, get_request](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);

            if (list.empty()) return std::string("");

            return *list.front();
        };
        return true;
    }
    else if (index_str == "B")
    {
        if (!manager.HasMapInQueue(Components.front()))
        {
            throw std::runtime_error(
                "Syntax error: Unable to get target map value from queue (Syntax: (Syntax: " + Expression
                + ", Line: " + std::to_string(LineNum) + ", FilePath: " + FilePath.string() + ")");
        }

        GetValueFunction = [&key, get_request](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);
            auto map_val = state.GetCurrentRequestMapValue(request, key);

            for (size_t i = 1; i < list.size(); i++)
            {
                if (list[i] == map_val) return *list[i - 1];
            }

            return *list.front();
        };
        return true;
    }
    else if (index_str == "N")
    {
        if (!manager.HasMapInQueue(Components.front()))
        {
            throw std::runtime_error(
                "Syntax error: Unable to get target map value from queue (Syntax: (Syntax: " + Expression
                + ", Line: " + std::to_string(LineNum) + ", FilePath: " + FilePath.string() + ")");
        }

        GetValueFunction = [&key, get_request](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);
            auto map_val = state.GetCurrentRequestMapValue(request, key);

            for (size_t i = 0; i < list.size() - 1; i++)
            {
                if (list[i] == map_val) return *list[i + 1];
            }

            return *list.back();
        };
        return true;
    }
    else if (index_str == "L")
    {
        GetValueFunction = [&key, get_request](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);

            if (list.empty()) return std::string("");

            return *list.back();
        };
        return true;
    }
    else if (isOnlyNumber(index_str))
    {
        size_t index = std::stoul(index_str);

        GetValueFunction = [&key, get_request, index](nemesis::CompileState& state)
        {
            auto request = (*get_request)(state);
            auto list    = request->GetMapValueList(key);

            if (list.empty()) return std::string("");

            return *list[index];
        };
        return true;
    }

    return false;
}

nemesis::MapStatement::MapStatement(const std::string& expression,
                                    size_t linenum,
                                    const std::filesystem::path& filepath,
                                    const nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 2:
        {
            const std::string& key  = Components.back();

            GetValueFunction = [&key](nemesis::CompileState& state)
            {
                auto request = state.GetBaseRequest();
                auto list    = request->GetMapValueList(key);

                if (list.empty()) return std::string("");

                return *list.front();
            };
            return;
        }
        case 3:
        {
            const std::string& index_str = Components[1];
            const std::string& key       = Components.back();
            auto get_request
                = std::make_shared<std::function<const nemesis::AnimationRequest*(nemesis::CompileState&)>>(
                    [](nemesis::CompileState& state) { return state.GetBaseRequest(); });

            if (SetupGetValueFunction(get_request, key, index_str, manager)) return;

            break;
        }
        case 4:
        {
            const std::string& key = Components[3];

            auto template_class    = manager.GetCurrentTemplateClass();
            auto get_request       = GetTargetRequest(*template_class, manager);

            GetValueFunction = [&key, get_request](nemesis::CompileState& state)
            {
                auto request = (*get_request)(state);
                auto list    = request->GetMapValueList(key);

                if (list.empty()) return std::string("");

                return *list.front();
            };
            return;
        }
        case 5:
        {
            const std::string& key       = Components[3];
            const std::string& index_str = Components.back();

            auto template_class    = manager.GetCurrentTemplateClass();
            auto get_request       = GetTargetRequest(*template_class, manager);

            if (SetupGetValueFunction(get_request, key, index_str, manager)) return;

            break;
        }
        default:
            break;
    }

    throw std::runtime_error("Syntax error: Unsupported Map components (Expression: " + expression
                             + ", Line: " + std::to_string(linenum) + ", File: " + filepath.string() + ")");
}

std::string nemesis::MapStatement::Serialize() const
{
    return Expression;
}

std::string nemesis::MapStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

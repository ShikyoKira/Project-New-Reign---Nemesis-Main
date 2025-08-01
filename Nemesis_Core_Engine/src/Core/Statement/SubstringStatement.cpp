#include "Core/Statement/SubstringStatement.h"

#include "Utilities/Algorithm.h"

void nemesis::SubstringStatement::Parse3Components(const nemesis::SemanticManager& manager)
{
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_value;
    UPtr<std::function<size_t(nemesis::CompileState&)>> get_index;
    auto& value = Components[1];
    auto& index = Components.back();

    if (IsComplexComponent(value))
    {
        auto& dynamic_value = DynamicComponents.emplace_back(value, LineNum, FilePath, manager);
        get_value           = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_value](nemesis::CompileState& state) { return dynamic_value.GetValue(state); });
    }
    else
    {
        get_value = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&value](nemesis::CompileState& state) { return value; });
    }

    if (IsComplexComponent(index))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index, LineNum, FilePath, manager);
        get_index           = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [this, &dynamic_index](nemesis::CompileState& state)
            {
                std::string index = dynamic_index.GetValue(state);

                if (is_only_number(index)) return std::stoul(index);

                ThrowSyntaxError("Invalid position value for Substring (" + index + ")");
            });
    }
    else
    {
        if (!is_only_number(index))
        {
            ThrowSyntaxError("Invalid position value for Substring (" + index + ")");
        }

        size_t i_index = std::stoul(index);
        get_index      = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [i_index](nemesis::CompileState& state) { return i_index; });
    }

    GetValueFunction
        = [get_value = std::move(get_value), get_index = std::move(get_index)](nemesis::CompileState& state)
    { return (*get_value)(state).substr((*get_index)(state)); };
}

void nemesis::SubstringStatement::Parse4Components(const nemesis::SemanticManager& manager)
{
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_value;
    UPtr<std::function<size_t(nemesis::CompileState&)>> get_index_1;
    UPtr<std::function<size_t(nemesis::CompileState&)>> get_index_2;
    auto& value   = Components[1];
    auto& index_1 = Components[2];
    auto& index_2 = Components.back();

    if (IsComplexComponent(value))
    {
        auto& dynamic_value = DynamicComponents.emplace_back(value, LineNum, FilePath, manager);
        get_value           = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_value](nemesis::CompileState& state) { return dynamic_value.GetValue(state); });
    }
    else
    {
        get_value = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&value](nemesis::CompileState& state) { return value; });
    }

    if (IsComplexComponent(index_1))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index_1, LineNum, FilePath, manager);
        get_index_1         = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [this, &dynamic_index](nemesis::CompileState& state)
            {
                std::string index = dynamic_index.GetValue(state);

                if (is_only_number(index)) return std::stoul(index);

                ThrowSyntaxError("Invalid position value for Substring (" + index + ")");
            });
    }
    else
    {
        if (!is_only_number(index_1))
        {
            ThrowSyntaxError("Invalid position value for Substring (" + index_1 + ")");
        }

        size_t i_index = std::stoul(index_1);
        get_index_1    = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [i_index](nemesis::CompileState& state) { return i_index; });
    }

    if (IsComplexComponent(index_2))
    {
        auto& dynamic_index = DynamicComponents.emplace_back(index_2, LineNum, FilePath, manager);
        get_index_2         = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [this, &dynamic_index](nemesis::CompileState& state)
            {
                std::string index = dynamic_index.GetValue(state);

                if (is_only_number(index)) return std::stoul(index);

                ThrowSyntaxError("Invalid position value for Substring (" + index + ")");
            });
    }
    else
    {
        if (!is_only_number(index_2))
        {
            ThrowSyntaxError("Invalid position value for Substring (" + index_1 + ")");
        }

        size_t i_index = std::stoul(index_2);
        get_index_2    = std::make_unique<std::function<size_t(nemesis::CompileState&)>>(
            [i_index](nemesis::CompileState& state) { return i_index; });
    }

    GetValueFunction = [get_value   = std::move(get_value),
                        get_index_1 = std::move(get_index_1),
                        get_index_2 = std::move(get_index_2)](
                           nemesis::CompileState& state)
    { return (*get_value)(state).substr((*get_index_1)(state), (*get_index_2)(state)); };
}

nemesis::SubstringStatement::SubstringStatement(const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath,
                                                const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    switch (Components.size())
    {
        case 3:
        {
            Parse3Components(manager);
            break;
        }
        case 4:
        {
            Parse4Components(manager);
            break;
        }
        default:
            ThrowSyntaxError("Unsupported Substring format");
    }
}

std::string nemesis::SubstringStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

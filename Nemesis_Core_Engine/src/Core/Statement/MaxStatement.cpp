#include "Core/Statement/MaxStatement.h"

#include "Utilities/Algorithm.h"

bool nemesis::MaxStatement::TryParse3Components(const nemesis::SemanticManager& manager)
{
    const std::string& val_1 = Components[1];
    const std::string& val_2 = Components.back();
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_val_1;

    if (IsComplexComponent(val_1))
    {
        auto& dynamic_val_1 = DynamicComponents.emplace_back(val_1, LineNum, FilePath, manager);
        get_val_1           = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_val_1](nemesis::CompileState& state)
            { return dynamic_val_1.GetValue(state); });
    }
    else if (is_only_number(val_1))
    {
        get_val_1 = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [val_1](nemesis::CompileState& state) { return val_1; });
    }
    else
    {
        return false;
    }

    if (IsComplexComponent(val_2))
    {
        auto& dynamic_val_2 = DynamicComponents.emplace_back(val_2, LineNum, FilePath, manager);
        GetValueFunction
            = [this, get_val_1 = std::move(get_val_1), &dynamic_val_2](nemesis::CompileState& state)
        {
            std::string val_1 = (*get_val_1)(state);
            std::string val_2 = dynamic_val_2.GetValue(state);

            if (!is_only_number(val_1))
            {
                ThrowInvalidError("Invalid number value (" + val_1 + ")");
            }

            if (!is_only_number(val_2))
            {
                ThrowInvalidError("Invalid number value (" + val_2 + ")");
            }

            return std::stod(val_1) > std::stod(val_2) ? val_1 : val_2;
        };
        return true;
    }
    else if (is_only_number(val_2))
    {
        GetValueFunction = [this, get_val_1 = std::move(get_val_1), val_2](nemesis::CompileState& state)
        {
            std::string val_1 = (*get_val_1)(state);

            if (!is_only_number(val_1))
            {
                ThrowInvalidError("Invalid number value (" + val_1 + ")");
            }

            return std::stod(val_1) > std::stod(val_2) ? val_1 : val_2;
        };
        return true;
    }

    return false;
}

nemesis::MaxStatement::MaxStatement(const std::string& expression,
                                    size_t linenum,
                                    const std::filesystem::path& filepath,
                                    const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() == 3)
    {
        if (TryParse3Components(manager)) return;
    }

    ThrowSyntaxError("Unsupported Max components");
}

std::string nemesis::MaxStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

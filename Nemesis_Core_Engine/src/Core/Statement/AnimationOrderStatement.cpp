#include "Core/Statement/AnimationOrderStatement.h"

#include "Core/CompileState.h"

nemesis::AnimationOrderStatement::AnimationOrderStatement(const std::string& expression,
                                                          size_t linenum,
                                                          const std::filesystem::path& filepath,
                                                          const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 3)
    {
        throw std::runtime_error("Syntax Error: AnimationOrder only accepts 1 argument (Syntax: "
                                 + expression + ", Line: " + std::to_string(linenum)
                                 + ", File: " + filepath.string() + ")");
    }

    auto& char_path = Components[1];
    auto& anim_path = Components.back();

    if (IsComplexComponent(char_path))
    {
        auto& dynamic_char_path = DynamicComponents.emplace_back(char_path, linenum, filepath, manager);
        auto get_char_path      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_char_path](nemesis::CompileState& state) { return dynamic_char_path.GetValue(state); });

        if (IsComplexComponent(anim_path))
        {
            auto& dynamic_anim_path = DynamicComponents.emplace_back(anim_path, linenum, filepath, manager);
            auto get_anim_path      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
                [&dynamic_anim_path](nemesis::CompileState& state)
                { return dynamic_anim_path.GetValue(state); });
            GetValueFunction = [&get_char_path, &get_anim_path](nemesis::CompileState& state)
            {
                auto char_path = (*get_char_path)(state);
                auto anim_path = (*get_anim_path)(state);
                return std::to_string(state.GetAnimationOrder(char_path, anim_path));
            };
            return;
        }

        GetValueFunction = [&get_char_path, &anim_path](nemesis::CompileState& state)
        {
            auto char_path = (*get_char_path)(state);
            return std::to_string(state.GetAnimationOrder(char_path, anim_path));
        };
        return;
    }
    
    if (!IsComplexComponent(anim_path))
    {
        GetValueFunction = [&char_path, &anim_path](nemesis::CompileState& state)
        { return std::to_string(state.GetAnimationOrder(char_path, anim_path)); };
        return;
    }

    auto& dynamic_anim_path = DynamicComponents.emplace_back(anim_path, linenum, filepath, manager);
    auto get_anim_path      = std::make_shared<std::function<std::string(nemesis::CompileState&)>>(
        [&dynamic_anim_path](nemesis::CompileState& state) { return dynamic_anim_path.GetValue(state); });
    GetValueFunction = [&char_path, &get_anim_path](nemesis::CompileState& state)
    {
        auto anim_path = (*get_anim_path)(state);
        return std::to_string(state.GetAnimationOrder(char_path, anim_path));
    };
}

std::string nemesis::AnimationOrderStatement::GetValue(nemesis::CompileState& state) const
{
    return GetValueFunction(state);
}

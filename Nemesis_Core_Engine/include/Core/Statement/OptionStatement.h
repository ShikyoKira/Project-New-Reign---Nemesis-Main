#pragma once

#include <functional>

#include "Core/Statement/CompositeStatement.h"

#include "Core/Template/TemplateOption.h"

namespace nemesis
{
    struct OptionStatement : public nemesis::CompositeStatement
    {
    private:
        using OptionPairList = Vec<Pair<const nemesis::TemplateOption*, int>>;

        std::function<OptionPairList(nemesis::CompileState&)> GetOptionsFunction;
        std::function<bool(nemesis::CompileState&)> HasOptionFunction;

        SPtr<std::function<nemesis::OptionStatement::OptionPairList(const nemesis::AnimationRequest*,
                                                                    nemesis::CompileState&)>>
        BuildGetOptionsFunction(const std::string& option_syntax,
                                const std::string& option_name,
                                const std::string& index,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager);
        SPtr<std::function<nemesis::OptionStatement::OptionPairList(nemesis::CompileState&)>>
        BuildGetBaseOptionsFunction(const std::string& option_name,
                                    const std::string& index_str,
                                    size_t linenum,
                                    const std::filesystem::path& filepath,
                                    const nemesis::SemanticManager& manager);
        std::function<bool(nemesis::CompileState&)>
        BuildAnyHasOptionFunction(const std::string& option_name, const nemesis::SemanticManager& manager);
        std::function<bool(nemesis::CompileState&)>
        BuildAllHasOptionFunction(const std::string& option_name, const nemesis::SemanticManager& manager);

    public:
        OptionStatement(const std::string& expression,
                        size_t linenum,
                        const std::filesystem::path& filepath,
                        const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        Pair<const nemesis::TemplateOption*, int> GetValue(nemesis::CompileState& state) const;
        Vec<Pair<const nemesis::TemplateOption*, int>> GetValues(nemesis::CompileState& state) const;
        int GetSize(nemesis::CompileState& state) const;
        const nemesis::TemplateOption* GetOption(nemesis::CompileState& state) const;
        std::string GetVariableValue(nemesis::CompileState& state, const std::string& variable_name) const;
        bool HasOption(nemesis::CompileState& state) const;
    };
}

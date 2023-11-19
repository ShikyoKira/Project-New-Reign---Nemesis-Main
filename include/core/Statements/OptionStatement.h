#pragma once

#include <functional>

#include "core/Statements/CompositeStatement.h"

#include "core/Template/TemplateOption.h"

namespace nemesis
{
	struct OptionStatement : public nemesis::CompositeStatement
    {
    private:
        std::function<const nemesis::TemplateOption*(nemesis::CompileState&)> GetOptionFunction;
        std::function<bool(nemesis::CompileState&)> HasOptionFunction;

        SPtr<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*,
                                                          nemesis::CompileState&)>>
        BuildGetOptionFunction(const std::string& option_syntax,
                               const std::string& option_name,
                               const std::string& index,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);
        SPtr<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>
        BuildGetBaseOptionFunction(const std::string& option_name,
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
        std::string GetValue(nemesis::CompileState& state) const override;

        const nemesis::TemplateOption* GetOption(nemesis::CompileState& state) const;
        std::string GetVariableValue(nemesis::CompileState& state, const std::string& variable_name) const;
        bool HasOption(nemesis::CompileState& state) const;
    };
}

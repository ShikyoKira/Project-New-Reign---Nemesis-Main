#pragma once

#include "core/SemanticManager.h"

#include "core/Statements/CompositeStatement.h"

namespace nemesis
{
    struct TemplateOption;
    struct TemplateOptionModel;
    struct OptionModel;
    struct CompileState;
    struct AnimationRequest;
    struct LineModifier;

	struct OptionVariableStatement : public nemesis::CompositeStatement
    {
    private:
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

        SPtr<std::function<const nemesis::TemplateOption*(const nemesis::AnimationRequest*,
                                                          nemesis::CompileState&)>>
        GetOptionFunction(const std::string& option_syntax,
                          const std::string& option_name,
                          const std::string& index,
                          size_t linenum,
                          const std::filesystem::path& filepath,
                          const nemesis::SemanticManager& manager);
        SPtr<std::function<const nemesis::TemplateOption*(nemesis::CompileState&)>>
        GetBaseOptionFunction(const std::string& option_name,
                              const std::string& index_str,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              const nemesis::SemanticManager& manager);
        SPtr<std::function<std::string(nemesis::CompileState&)>>
        GetVariableFunction(const std::string& variable,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager,
                            const nemesis::TemplateOptionModel* model);

    public:
        OptionVariableStatement(const std::string& expression,
                                size_t linenum,
                                const std::filesystem::path& filepath,
                                const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const;
    };
}

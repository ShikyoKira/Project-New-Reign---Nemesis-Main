#pragma once

#include "Core/SemanticManager.h"

#include "Core/Statement/OptionStatement.h"
#include "Core/Statement/CompositeStatement.h"

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
        nemesis::OptionStatement OptionStatement;
        std::function<std::string(nemesis::CompileState&)> GetValueFunction;

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

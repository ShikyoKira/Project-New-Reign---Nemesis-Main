#pragma once

#include "core/Statements/Statement.h"

namespace nemesis
{
    struct LineModifier;
    struct SemanticManager;

	struct MathStatement : public nemesis::Statement
    {
    private:
        VecStr Tokens;
        Vec<UPtr<nemesis::LineModifier>> Modifiers;

        std::function<std::string(nemesis::CompileState&)> GetEquationFunction;


    public:
        MathStatement(const std::string& expression,
                      size_t linenum,
                      const std::filesystem::path& filepath,
                      const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const;
    };
}

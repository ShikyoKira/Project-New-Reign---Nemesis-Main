#pragma once

#include "core/Statements/CompositeStatement.h"

namespace nemesis
{
    struct LineModifier;
    struct SemanticManager;

	struct MathStatement : public nemesis::CompositeStatement
    {
    public:
        MathStatement(const std::string& expression,
                      size_t linenum,
                      const std::filesystem::path& filepath,
                      const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        std::string GetValue(nemesis::CompileState& state) const override;
    };
}

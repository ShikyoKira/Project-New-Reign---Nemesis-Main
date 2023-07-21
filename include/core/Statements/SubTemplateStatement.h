#pragma once

#include "core/Statements/CompositeStatement.h"

namespace nemesis
{
    struct LineModifier;
    struct SemanticManager;

    struct SubTemplateStatement : public nemesis::CompositeStatement
    {
    public:
        SubTemplateStatement(const std::string& expression,
                             size_t linenum,
                             const std::filesystem::path& filepath,
                             const nemesis::SemanticManager& manager);

        std::string Serialize() const override;

        VecStr GetComponents(nemesis::CompileState& state) const;
        std::string GetValue(nemesis::CompileState& state) const;
    };
}

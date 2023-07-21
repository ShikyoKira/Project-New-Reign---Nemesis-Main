#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/SubTemplateStatement.h"

namespace nemesis
{
    struct SubTemplateModifier : public nemesis::LineModifier
    {
    private:
        SubTemplateStatement Statement;

    public:
        SubTemplateModifier(size_t begin,
                            size_t end,
                            const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::SubTemplateStatement* GetStatement() const noexcept;
    };
}

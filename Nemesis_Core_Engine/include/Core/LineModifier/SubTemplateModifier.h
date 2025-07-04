#pragma once

#include "Core/LineModifier/LineModifier.h"

#include "Core/Statement/SubTemplateStatement.h"

namespace nemesis
{
    struct SubTemplateModifier : public nemesis::LineModifier
    {
    private:
        SPtr<nemesis::SubTemplateStatement> Statement;

    public:
        SubTemplateModifier(size_t begin,
                            size_t end,
                            const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::SubTemplateStatement& GetStatement() const noexcept;
    };
}

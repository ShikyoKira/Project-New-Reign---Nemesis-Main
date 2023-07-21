#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/MultipleChoiceStatement.h"

namespace nemesis
{
    struct Template;

    struct MultipleChoiceModifier : public nemesis::LineModifier
    {
    private:
        nemesis::MultipleChoiceStatement Statement;

    public:
        MultipleChoiceModifier(size_t begin,
                               size_t end,
                               const std::string& line,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::MultipleChoiceStatement* GetStatement() const noexcept;
    };
}

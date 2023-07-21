#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/OptionVariableStatement.h"

namespace nemesis
{
	struct OptionVariableModifier : public nemesis::LineModifier
    {
    private:
        nemesis::OptionVariableStatement statement;

    public:
        OptionVariableModifier(size_t begin,
                               size_t end,
                               const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;

        const nemesis::OptionVariableStatement* GetStatement() const noexcept;
    };
}

#pragma once

#include "core/LineModifier/LineModifier.h"

#include "core/Statements/MathStatement.h"

namespace nemesis
{
	struct MathModifier : public nemesis::LineModifier
    {
    private:
        MathStatement Statement;

    public:
        MathModifier(size_t begin,
                     size_t end,
                     const std::string& expression,
                     size_t linenum,
                     const std::filesystem::path& filepath,
                     const nemesis::SemanticManager& manager);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const;

        const nemesis::MathStatement* GetStatement() const noexcept;

    };
}

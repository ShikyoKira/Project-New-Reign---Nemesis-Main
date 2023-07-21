#pragma once

#include "utilities/line.h"

#include "core/LineModifier/LineModifierFactory.h"

namespace nemesis
{
    struct CompileState;

	struct LineModifier
    {
    protected:
        size_t Begin;
        size_t End;

        std::string GetCoveredBlocks(const VecStr& blocks) const;
        void ClearCoveredBlocks(VecStr& blocks) const;

    public:
        LineModifier(size_t begin, size_t end) noexcept;

        virtual void Apply(VecStr& blocks, nemesis::CompileState& state) const = 0;
    };
}

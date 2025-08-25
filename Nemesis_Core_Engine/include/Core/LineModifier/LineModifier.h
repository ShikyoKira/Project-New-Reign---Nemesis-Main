#pragma once

#include "Utilities/Exception.h"
#include "Utilities/Line.h"

#include "Core/LineModifier/LineModifierFactory.h"

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

        virtual ~LineModifier() noexcept {}

        virtual void Apply(VecStr& blocks, nemesis::CompileState& state) const = 0;
    };

    struct ModifierException : nemesis::Exception
    {
        ModifierException(const std::string& msg)
            : nemesis::Exception(msg)
        {
        }
    };
}

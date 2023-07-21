#pragma once

#include "core/LineModifier/LineModifier.h"

namespace nemesis
{
	struct GroupSelectionModifier : public nemesis::LineModifier
    {
    private:
        UPtr<nemesis::LineModifier> modifier;

    public:
        GroupSelectionModifier(size_t begin, size_t end) noexcept;

        void AddModifier(UPtr<nemesis::LineModifier> modifier);

        void Apply(VecStr& blocks, nemesis::CompileState& state) const override;
    };
}

#pragma once

#include "core/LineModifier/LineModifier.h"

namespace nemesis
{
	struct LineModifierCollection
    {
    private:
        Vec<UPtr<nemesis::LineModifier>> Modifiers;

    public:
        void AddModifier(UPtr<nemesis::LineModifier> modifier);
        nemesis::Line Apply(const nemesis::Line& line, nemesis::CompileState& state);
    };
}

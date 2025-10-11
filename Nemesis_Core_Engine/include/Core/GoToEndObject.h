#pragma once

#include "Core/NObject.h"

namespace nemesis
{
    struct GoToEndObject : nemesis::NObject
    {
    public:
        GoToEndObject() = default;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::GoToEndObject> Clone() const;
    };
}

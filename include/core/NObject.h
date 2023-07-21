#pragma once

#include "utilities/line.h"

namespace nemesis
{
    struct CompileState;
    struct NLine;

	struct NObject
    {
    public:
        virtual void CompileTo(DeqNstr&, nemesis::CompileState&) const = 0;
        virtual void SerializeTo(DeqNstr&) const                       = 0;

        virtual DeqNstr Compile(nemesis::CompileState& state) const;
        virtual DeqNstr Serialize() const;
    };
}

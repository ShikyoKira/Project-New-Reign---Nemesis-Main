#include "core/NLine.h"
#include "core/CompileState.h"

DeqNstr nemesis::NObject::Compile(nemesis::CompileState& state) const
{
    DeqNstr lines;
    CompileTo(lines, state);
    return lines;
}

DeqNstr nemesis::NObject::Serialize() const
{
    DeqNstr lines;
    SerializeTo(lines);
    return lines;
}

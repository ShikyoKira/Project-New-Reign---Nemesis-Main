#include "Core/GoToEndObject.h"

#include "Utilities/ConditionSyntax.h"

namespace ns = nemesis::syntax;

void nemesis::GoToEndObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
}

void nemesis::GoToEndObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(ns::GoToEnd());
}

UPtr<nemesis::NObject> nemesis::GoToEndObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::GoToEndObject> nemesis::GoToEndObject::Clone() const
{
    return std::make_unique<nemesis::GoToEndObject>();
}

#include "core/EmptyObject.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

void nemesis::EmptyObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    return;
}

void nemesis::EmptyObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(ns::DeleteLine());
}

UPtr<nemesis::NObject> nemesis::EmptyObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::EmptyObject> nemesis::EmptyObject::Clone() const
{
    return std::make_unique<nemesis::EmptyObject>();
}

bool nemesis::EmptyObject::Match(const std::string& line) noexcept
{
    return line.find(ns::DeleteLine()) != NOT_FOUND;
}

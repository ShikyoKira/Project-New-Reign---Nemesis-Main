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

bool nemesis::EmptyObject::Match(const std::string& line) noexcept
{
    return line.find(ns::DeleteLine()) != NOT_FOUND;
}

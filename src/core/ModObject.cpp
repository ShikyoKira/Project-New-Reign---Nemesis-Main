#include "core/ModObject.h"
#include "core/CompileState.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::ModObject::ModObject(const nemesis::ModObject& modobject)
    : Statement(modobject.Statement)
{
    Value = modobject.Value->CloneNObject();
}

nemesis::ModObject::ModObject(const std::string& modcode,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              nemesis::SemanticManager& manager,
                              UPtr<nemesis::NObject>&& value)
    : Statement(modcode, linenum, filepath, manager)
    , Value(std::move(value))
{
}

void nemesis::ModObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    if (!Statement.IsTrue(state)) return;

    if (!Value) return;

    Value->CompileTo(lines, state);
}

void nemesis::ModObject::SerializeTo(DeqNstr& lines) const
{
    std::string modcode(Statement.GetExpression());
    lines.emplace_back(ns::ModCode(modcode));

    if (Value)
    {
        Value->SerializeTo(lines);
    }
    else
    {
        lines.emplace_back(ns::DeleteLine());
    }

    lines.emplace_back(ns::ModClose());
}

UPtr<nemesis::NObject> nemesis::ModObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::ModObject> nemesis::ModObject::Clone() const
{
    return UPtr<nemesis::ModObject>(new nemesis::ModObject(*this));
}

const nemesis::ModCodeStatement& nemesis::ModObject::GetStatement() const noexcept
{
    return Statement;
}

bool nemesis::ModObject::IsSelected(nemesis::CompileState& state) const
{
    return state.IsModSelected(Statement.GetExpression());
}

std::string_view nemesis::ModObject::GetModCode() const noexcept
{
    return Statement.GetExpression();
}

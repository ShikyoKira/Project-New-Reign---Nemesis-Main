#include "Core/IfObject.h"
#include "Core/SemanticManager.h"

#include "Utilities/ConditionSyntax.h"

namespace ns = nemesis::syntax;

nemesis::IfObject::ElseIfObject::ElseIfObject(const nemesis::IfObject::ElseIfObject& elseif_obj)
    : Statement(elseif_obj.Statement)
    , Value(elseif_obj.Value->CloneNObject())
{
}

nemesis::IfObject::ElseIfObject::ElseIfObject(const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager,
                                              UPtr<nemesis::NObject>&& value)
    : Statement(expression, linenum, filepath, manager)
    , Value(std::move(value))
{
}

UPtr<nemesis::IfObject::ElseIfObject> nemesis::IfObject::ElseIfObject::Clone() const
{
    return UPtr<nemesis::IfObject::ElseIfObject>(new nemesis::IfObject::ElseIfObject(*this));
}

nemesis::IfObject::IfObject(const nemesis::IfObject& if_obj)
    : Statement(if_obj.Statement)
{
    Value = if_obj.Value->CloneNObject();

    for (auto& elseif_obj : if_obj.ElseIfCollection)
    {
        ElseIfCollection.emplace_back(elseif_obj->Clone());
    }

    if (!if_obj.ElseValue) return;

    ElseValue = if_obj.ElseValue->CloneNObject();
}

nemesis::IfObject::IfObject(const std::string& expression,
                            size_t linenum,
                            const std::filesystem::path& filepath,
                            const nemesis::SemanticManager& manager,
                            UPtr<nemesis::NObject>&& value)
    : Statement(expression, linenum, filepath, manager)
    , Value(std::move(value))
{
}

void nemesis::IfObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    if (Statement.IsTrue(state))
    {
        Value->CompileTo(lines, state);
        return;
    }

    for (auto& elseif_obj : ElseIfCollection)
    {
        if (!elseif_obj->Statement.IsTrue(state)) continue;

        elseif_obj->Value->CompileTo(lines, state);
        return;
    }

    if (!ElseValue) return;

    ElseValue->CompileTo(lines, state);
}

void nemesis::IfObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(Statement.Serialize());
    Value->SerializeTo(lines);

    for (auto& elseif_obj : ElseIfCollection)
    {
        lines.emplace_back(elseif_obj->Statement.Serialize());
        Value->SerializeTo(lines);
    }

    if (ElseValue)
    {
        lines.emplace_back(ns::Else());
        ElseValue->SerializeTo(lines);
    }

    lines.emplace_back(ns::EndIf());
}

UPtr<nemesis::NObject> nemesis::IfObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::IfObject> nemesis::IfObject::Clone() const
{
    return UPtr<nemesis::IfObject>(new nemesis::IfObject(*this));
}

const nemesis::IfStatement& nemesis::IfObject::GetStatement() const noexcept
{
    return Statement;
}

void nemesis::IfObject::ElseIf(const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager,
                               UPtr<nemesis::NObject>&& value)
{
    ElseIfCollection.emplace_back(std::make_unique<nemesis::IfObject::ElseIfObject>(expression, linenum, filepath, manager, std::move(value)));
}

void nemesis::IfObject::Else(UPtr<nemesis::NObject>&& value) noexcept
{
    ElseValue = std::move(value);
}

#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/SemanticManager.h"

#include "utilities/conditionsyntax.h"
#include "..\..\include\core\ForEachObject.h"

namespace ns = nemesis::syntax;

nemesis::ForEachObject::ForEachObject(const nemesis::ForEachObject& fe_object)
    : Statement(fe_object.Statement)
{
    Value = fe_object.Value->CloneNObject();
}

nemesis::ForEachObject::ForEachObject(const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      nemesis::SemanticManager& manager,
                                      UPtr<nemesis::NObject>&& value)
    : Statement(expression, linenum, filepath, manager)
    , Value(std::move(value))
{
}

void nemesis::ForEachObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    Statement.ForEach(state,
                      [this, &lines, &state]()
                      {
                          Value->CompileTo(lines, state);
                      });
}

void nemesis::ForEachObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(Statement.Serialize());
    Value->SerializeTo(lines);
    lines.emplace_back(ns::Close());
}

UPtr<nemesis::NObject> nemesis::ForEachObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::ForEachObject> nemesis::ForEachObject::Clone() const
{
    return UPtr<nemesis::ForEachObject>(new nemesis::ForEachObject(*this));
}

const nemesis::ForEachStatement& nemesis::ForEachObject::GetStatement() const noexcept
{
    return Statement;
}

UPtr<nemesis::ParsingForEachScope> nemesis::ForEachObject::BuildScope(nemesis::SemanticManager& manager) const
{
    return std::make_unique<nemesis::ParsingForEachScope>(Statement, manager);
}

bool nemesis::ForEachObject::MatchForEach(const std::string& line, std::string& condition)
{
    try
    {
        condition = std::string(ns::ForEachCondition(line));
        return true;
    }
    catch (const std::exception&)
    {
        condition.clear();
        return false;
    }
}

bool nemesis::ForEachObject::MatchClose(const std::string& line) noexcept
{
    return line.find(ns::Close()) != NOT_FOUND;
}

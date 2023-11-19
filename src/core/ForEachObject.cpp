#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/SemanticManager.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::ForEachObject::ForEachObject(const std::string& expression,
                                      size_t linenum,
                                      const std::filesystem::path& filepath,
                                      nemesis::SemanticManager& manager,
                                      UPtr<nemesis::NObject>&& value)
    : Statement(expression, linenum, filepath, manager)
    , Value(std::move(value))
{
    manager.AddForEachToQueue(expression);
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

const nemesis::ForEachStatement& nemesis::ForEachObject::GetStatement() const noexcept
{
    return Statement;
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

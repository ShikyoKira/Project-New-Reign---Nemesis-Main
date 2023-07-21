#include "core/IfObject.h"
#include "core/SemanticManager.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::regex nemesis::IfObject::if_rgx("^.*\\<\\!-- IF \\^(.+?)\\^ --\\>.*$");
nemesis::regex nemesis::IfObject::elseif_rgx("^.*\\<\\!-- ELSEIF \\^(.+?)\\^ --\\>.*$");

nemesis::IfObject::ElseIfObject::ElseIfObject(const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager,
                                              UPtr<nemesis::NObject>&& value)
    : Statement(expression, linenum, filepath, manager)
    , Value(std::move(value))
{
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
    if (!Statement.IsTrue(state)) return;

    Value->CompileTo(lines, state);
}

void nemesis::IfObject::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(Statement.Serialize());
    Value->SerializeTo(lines);
    lines.emplace_back(ns::EndIf());
}

void nemesis::IfObject::ElseIf(const std::string& expression,
                               size_t linenum,
                               const std::filesystem::path& filepath,
                               const nemesis::SemanticManager& manager,
                               UPtr<nemesis::NObject>&& value)
{
    ElseIfCollection.emplace_back(expression, linenum, filepath, manager, std::move(value));
}

void nemesis::IfObject::Else(UPtr<nemesis::NObject>&& value) noexcept
{
    ElseValue = std::move(value);
}

bool nemesis::IfObject::MatchIf(const std::string& line, std::string& condition)
{
    try
    {
        condition = std::string(ns::IfCondition(line));
        return true;
    }
    catch (const std::exception&)
    {
        condition.clear();
        return false;
    }

    //nemesis::smatch match;

    //if (nemesis::regex_match(line, match, if_rgx))
    //{
    //    condition = match.str(1);
    //    return true;
    //}

    //condition.clear();
    //return false;
}

bool nemesis::IfObject::MatchElseIf(const std::string& line, std::string& condition)
{
    try
    {
        condition = std::string(ns::ElseIfCondition(line));
        return true;
    }
    catch (const std::exception&)
    {
        condition.clear();
        return false;
    }

    //nemesis::smatch match;

    //if (nemesis::regex_match(line, match, elseif_rgx))
    //{
    //    condition = match.str(1);
    //    return true;
    //}

    //condition.clear();
    //return false;
}

bool nemesis::IfObject::MatchEndIf(const std::string& line) noexcept
{
    return line.find(ns::EndIf()) != NOT_FOUND;
}

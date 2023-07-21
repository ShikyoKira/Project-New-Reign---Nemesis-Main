#include "core/ModLine.h"
#include "core/CompileState.h"

#include "utilities/regex.h"
#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::ModLine::ModLine(const std::string& modcode,
                          size_t linenum,
                          const std::filesystem::path& filepath,
                          nemesis::SemanticManager& manager)
    : Statement(modcode, linenum, filepath, manager)
{
}

nemesis::ModLine::ModLine(const std::string& modcode,
                          size_t linenum,
                          const std::filesystem::path& filepath,
                          nemesis::SemanticManager& manager,
                          const nemesis::Line& line)
    : nemesis::NLine(line, manager)
    , Statement(modcode, linenum, filepath, manager)
{
}

void nemesis::ModLine::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    if (!Statement.IsTrue(state)) return;

    nemesis::NLine::CompileTo(lines, state);
}

void nemesis::ModLine::SerializeTo(DeqNstr& lines) const
{
    std::string modcode = std::string(Statement.GetExpression());

    if (Value)
    {
        lines.emplace_back(*Value + ns::Spaces() + ns::Aster(modcode));
        return;
    }

    lines.emplace_back(ns::DeleteLine() + ns::Spaces() + ns::Aster(modcode));
}

void nemesis::ModLine::AddModLine(const std::string& modcode,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  nemesis::SemanticManager& manager)
{
    throw std::runtime_error("Nested mod line is not supported");
}

void nemesis::ModLine::AddModLine(const std::string& modcode,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  nemesis::SemanticManager& manager,
                                  const nemesis::Line& line)
{
    throw std::runtime_error("Nested mod line is not supported");
}

bool nemesis::ModLine::IsSelected(nemesis::CompileState& state) const
{
    return state.IsModSelected(Statement.GetExpression());
}

std::string_view nemesis::ModLine::GetModCode() const
{
    return Statement.GetExpression();
}

bool nemesis::ModLine::MatchOpen(const std::string& line, std::string& condition)
{
    try
    {
        condition = std::string(ns::ModCodeCondition(line));
        return true;
    }
    catch (const std::exception&)
    {
        condition.clear();
        return false;
    }

    //nemesis::smatch match;

    //if (nemesis::regex_match(line, match, "^.*\\<\\!-- MOD_CODE ~(.+?)~ OPEN --\\>.*$"))
    //{
    //    condition = match.str(1);
    //    return true;
    //}

    //condition.clear();
    //return false;
}

bool nemesis::ModLine::MatchOriginal(const std::string& line)
{
    return line.find(ns::ModOriginal()) != NOT_FOUND;
}

bool nemesis::ModLine::MatchClose(const std::string& line)
{
    return line.find(ns::ModClose()) != NOT_FOUND;
}

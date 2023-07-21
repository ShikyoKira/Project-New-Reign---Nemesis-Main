#include "core/Statements/IfStatement.h"

#include "utilities/conditionsyntax.h"

nemesis::IfStatement::IfStatement(const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(expression, linenum, filepath, manager)
{
}

nemesis::IfStatement::IfStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(line, manager)
{
    if (line.empty()) throw std::runtime_error("Empty IF STATEMENT is not allowed");

    Expression = line;
    LineNum    = line.GetLineNumber();
    size_t pos = line.find("!=");

    if (pos != NOT_FOUND)
    {
        Negative = true;
    }

    pos = line.find("==");

    if (pos != NOT_FOUND)
    {
        Negative = false;
    }

    Negative = line.front() == '!';
}

std::string nemesis::IfStatement::Serialize() const
{
    return nemesis::syntax::If(Expression);
}

bool nemesis::IfStatement::IsTrue(nemesis::CompileState& state) const
{
    return CondNode->IsTrue(state);
}

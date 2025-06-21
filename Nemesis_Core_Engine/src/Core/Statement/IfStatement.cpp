#include "Core/Statement/IfStatement.h"

#include "Utilities/ConditionSyntax.h"

nemesis::IfStatement::IfStatement(const std::string& expression,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(expression, linenum, filepath, manager)
{
    if (!expression.empty()) return;

    throw std::runtime_error("Empty IF STATEMENT is not allowed");
}

nemesis::IfStatement::IfStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(line, manager)
{
    if (!line.empty()) return;

    throw std::runtime_error("Empty IF STATEMENT is not allowed");
}

nemesis::IfStatement::IfStatement(const nemesis::IfStatement& statement)
    : nemesis::ConditionalStatement(statement)
{
}

std::string nemesis::IfStatement::Serialize() const
{
    return nemesis::syntax::If(Expression);
}

bool nemesis::IfStatement::IsTrue(nemesis::CompileState& state) const
{
    return CondNode->IsTrue(state);
}

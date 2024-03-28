#include "core/Statements/ElseIfStatement.h"

#include "utilities/conditionsyntax.h"

nemesis::ElseIfStatement::ElseIfStatement(const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::IfStatement(expression, linenum, filepath, manager)
{
}

nemesis::ElseIfStatement::ElseIfStatement(const nemesis::ElseIfStatement& statement)
    : nemesis::IfStatement(statement)
{
}

std::string nemesis::ElseIfStatement::Serialize() const
{
    return nemesis::syntax::ElseIf(Expression);
}
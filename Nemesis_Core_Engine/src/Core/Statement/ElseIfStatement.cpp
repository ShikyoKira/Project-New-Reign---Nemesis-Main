#include "Core/Statement/ElseIfStatement.h"

#include "Utilities/ConditionSyntax.h"

nemesis::ElseIfStatement::ElseIfStatement(const std::string& expression,
                                          size_t linenum,
                                          const std::filesystem::path& filepath,
                                          const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(expression, linenum, filepath, manager)
{
}

nemesis::ElseIfStatement::ElseIfStatement(const nemesis::Line& line, const nemesis::SemanticManager& manager)
    : nemesis::ConditionalStatement(line, manager)
{
}

nemesis::ElseIfStatement::ElseIfStatement(const nemesis::ElseIfStatement& statement)
    : nemesis::ConditionalStatement(statement)
{
}

std::string nemesis::ElseIfStatement::Serialize() const
{
    return nemesis::syntax::ElseIf(Expression);
}

bool nemesis::ElseIfStatement::IsTrue(nemesis::CompileState& state) const
{
    return CondNode->IsTrue(state);
}

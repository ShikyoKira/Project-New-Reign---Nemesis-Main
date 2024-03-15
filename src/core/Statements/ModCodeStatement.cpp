#include "core/Statements/ModCodeStatement.h"

#include "core/CompileState.h"

#include "utilities/conditionsyntax.h"

nemesis::ModCodeStatement::ModCodeStatement(const std::string& mod_code,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            nemesis::SemanticManager& manager) noexcept
{
    Expression = mod_code;
    LineNum    = linenum;
    FilePath   = filepath;
    
    manager.AddModInUsed(mod_code);
}

nemesis::ModCodeStatement::ModCodeStatement(const nemesis::Line& mod_code,
                                            nemesis::SemanticManager& manager) noexcept
{
    Expression = mod_code;
    LineNum    = mod_code.GetLineNumber();
    FilePath   = mod_code.GetFilePath();

    manager.AddModInUsed(mod_code);
}

nemesis::ModCodeStatement::ModCodeStatement(const nemesis::ModCodeStatement& statement)
    : nemesis::ConditionalStatement(statement)
{
}

std::string nemesis::ModCodeStatement::Serialize() const
{
    return nemesis::syntax::ModCode(Expression);
}

bool nemesis::ModCodeStatement::IsTrue(nemesis::CompileState& state) const
{
    return state.IsModSelected(Expression);
}

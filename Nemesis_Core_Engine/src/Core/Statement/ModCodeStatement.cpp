#include "Core/Statement/ModCodeStatement.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Utilities/ConditionSyntax.h"

nemesis::ModCodeStatement::ModCodeStatement(const std::string& modcode,
                                            size_t linenum,
                                            const std::filesystem::path& filepath) noexcept
    : nemesis::Statement(modcode, linenum, filepath)
{
}

nemesis::ModCodeStatement::ModCodeStatement(const std::string& modcode,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            nemesis::SemanticManager& manager) noexcept
    : nemesis::Statement(modcode, linenum, filepath)
{
    manager.AddModInUsed(modcode);
}

nemesis::ModCodeStatement::ModCodeStatement(const nemesis::Line& mod_code,
                                            nemesis::SemanticManager& manager) noexcept
    : nemesis::Statement(mod_code)
{
    Expression = mod_code;
    LineNum    = mod_code.GetLineNumber();
    FilePath   = mod_code.GetFilePath();

    manager.AddModInUsed(mod_code);
}

nemesis::ModCodeStatement::ModCodeStatement(const nemesis::ModCodeStatement& statement)
    : nemesis::Statement(statement)
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

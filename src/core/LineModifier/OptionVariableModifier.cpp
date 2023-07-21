#include "core/LineModifier/OptionVariableModifier.h"

nemesis::OptionVariableModifier::OptionVariableModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , statement(expression, linenum, filepath, manager)
{
}

void nemesis::OptionVariableModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = statement.GetValue(state);
}

const nemesis::OptionVariableStatement* nemesis::OptionVariableModifier::GetStatement() const noexcept
{
    return &statement;
}

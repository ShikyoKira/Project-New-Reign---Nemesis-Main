#include "core/LineModifier/RequestIdModifier.h"

#include "core/CompileState.h"

nemesis::RequestIdModifier::RequestIdModifier(size_t begin,
                                              size_t end,
                                              const std::string& expression,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , Statement(expression, linenum, filepath, manager)
{
}

void nemesis::RequestIdModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = Statement.GetValue(state);
}

const nemesis::RequestIdStatement* nemesis::RequestIdModifier::GetStatement() const noexcept
{
    return &Statement;
}

#include "core/LineModifier/RequestIndexModifier.h"

nemesis::RequestIndexModifier::RequestIndexModifier(size_t begin,
                                                    size_t end,
                                                    const std::string& expression,
                                                    size_t linenum,
                                                    const std::filesystem::path& filepath,
                                                    const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , Statement(expression, linenum, filepath, manager)
{
}

void nemesis::RequestIndexModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = Statement.GetValue(state);
}

const nemesis::RequestIndexStatement* nemesis::RequestIndexModifier::GetStatement() const noexcept
{
    return &Statement;
}

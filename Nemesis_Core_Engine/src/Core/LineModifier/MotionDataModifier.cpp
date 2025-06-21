#include "Core/LineModifier/MotionDataModifier.h"

nemesis::MotionDataModifier::MotionDataModifier(size_t begin,
                                                size_t end,
                                                const std::string& expression,
                                                size_t linenum,
                                                const std::filesystem::path& filepath,
                                                const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , Statement(expression, linenum, filepath, manager)
{
}

void nemesis::MotionDataModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = Statement.GetValue(state);
}

const nemesis::MotionDataStatement& nemesis::MotionDataModifier::GetStatement() const noexcept
{
    return Statement;
}

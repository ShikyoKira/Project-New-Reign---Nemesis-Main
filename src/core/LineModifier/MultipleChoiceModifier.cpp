#include "core/LineModifier/MultipleChoiceModifier.h"

nemesis::MultipleChoiceModifier::MultipleChoiceModifier(size_t begin,
                                                        size_t end,
                                                        const std::string& line,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::LineModifier(begin, end)
    , Statement(line, linenum, filepath, manager)
{
}

void nemesis::MultipleChoiceModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const
{
    ClearCoveredBlocks(blocks);
    blocks[Begin] = Statement.GetValue(blocks, state);
}

const nemesis::MultipleChoiceStatement* nemesis::MultipleChoiceModifier::GetStatement() const noexcept
{
    return &Statement;
}

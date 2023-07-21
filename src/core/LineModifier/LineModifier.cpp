#include "core/LineModifier/LineModifier.h"

std::string nemesis::LineModifier::GetCoveredBlocks(const VecStr& blocks) const
{
    std::string merged;

    for (size_t i = Begin; i <= End; ++i)
    {
        merged.append(blocks[i]);
    }

    return merged;
}

void nemesis::LineModifier::ClearCoveredBlocks(VecStr& blocks) const
{
    for (size_t i = Begin; i <= End; ++i)
    {
        blocks[i].clear();
    }
}

nemesis::LineModifier::LineModifier(size_t begin, size_t end) noexcept
{
    Begin = begin;
    End   = end;
}

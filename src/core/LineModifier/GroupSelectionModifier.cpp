#include "core/LineModifier/GroupSelectionModifier.h"

nemesis::GroupSelectionModifier::GroupSelectionModifier(size_t begin, size_t end) noexcept
    : nemesis::LineModifier(begin, end)
{
}

void nemesis::GroupSelectionModifier::AddModifier(UPtr<nemesis::LineModifier> modifier)
{
    this->modifier = std::move(modifier);
}

void nemesis::GroupSelectionModifier::Apply(VecStr& blocks, nemesis::CompileState& state) const 
{

}

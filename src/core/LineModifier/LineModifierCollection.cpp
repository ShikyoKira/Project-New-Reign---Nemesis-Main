#include "core/LineModifier/LineModifierCollection.h"

void nemesis::LineModifierCollection::AddModifier(UPtr<nemesis::LineModifier> modifier)
{
    Modifiers.emplace_back(std::move(modifier));
}

nemesis::Line nemesis::LineModifierCollection::Apply(const nemesis::Line& line, nemesis::CompileState& state)
{
    VecStr blocks;
    
    for (auto& ch : line)
    {
        blocks.push_back(std::string(1, ch));
    }

    for (auto& modifier : Modifiers)
    {
        modifier->Apply(blocks, state);
    }

    nemesis::Line new_line(line);
    new_line.clear();

    for (auto& block : blocks)
    {
        new_line.append(block);
    }

    return new_line;
}

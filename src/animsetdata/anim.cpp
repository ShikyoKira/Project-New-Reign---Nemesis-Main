#include "animsetdata/anim.h"

nemesis::animsetdata::Anim::Anim(const VecNstr& lines)
{}

nemesis::animsetdata::Anim::Anim(const Anim& anim) noexcept
    : name(anim.name)
    , unknown(anim.unknown)
    , attacklist(anim.attacklist)
{
}

VecNstr nemesis::animsetdata::Anim::GetLines() const
{
    VecNstr lines;
    name->GetRawData(lines);
    unknown->GetRawData(lines);
    lines.emplace_back(std::to_string(attacklist.size()));

    for (auto& attack : attacklist)
    {
        attack.GetRawData(lines);
    }

    return lines;
}

VecNstr nemesis::animsetdata::Anim::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    VecNstr attacklines;
    lines.emplace_back(name->GetProcessedLines(scopeinfo).front());
    lines.emplace_back(unknown->GetProcessedLines(scopeinfo).front());

    for (auto& attack : attacklist)
    {
        auto compiledattacks = attack.GetProcessedLines(scopeinfo);
        attacklines.insert(attacklines.end(), compiledattacks.begin(), compiledattacks.end());
    }

    lines.emplace_back(std::to_string(attacklines.size()));
    lines.insert(lines.end(), attacklines.begin(), attacklines.end());
    return lines;
}

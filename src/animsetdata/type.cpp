#include "animsetdata/type.h"

nemesis::animsetdata::Type::Type(const VecNstr& lines)
{}

nemesis::animsetdata::Type::Type(const Type& type) noexcept
    : name(type.name)
    , equiptype1(type.equiptype1)
    , equiptype2(type.equiptype2)
{
}

VecNstr nemesis::animsetdata::Type::GetLines() const
{
    VecNstr lines;
    name->GetRawData(lines);
    equiptype1->GetRawData(lines);
    equiptype2->GetRawData(lines);
    return lines;
}

VecNstr nemesis::animsetdata::Type::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    lines.emplace_back(name->GetProcessedLines(scopeinfo).front());
    lines.emplace_back(equiptype1->GetProcessedLines(scopeinfo).front());
    lines.emplace_back(equiptype2->GetProcessedLines(scopeinfo).front());
    return lines;
}

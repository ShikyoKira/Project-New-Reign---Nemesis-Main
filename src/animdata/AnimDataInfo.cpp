#include "animdata/AnimDataInfo.h"

#include "core/CompileState.h"

void nemesis::animdata::AnimDataInfo::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    Name.CompileTo(lines, state);
    UniqueCode.CompileTo(lines, state);
    PlaybackSpeed.CompileTo(lines, state);
    CropStartAmountLocalTime.CompileTo(lines, state);
    CropEndAmountLocalTime.CompileTo(lines, state);

    lines.emplace_back(std::to_string(EventNames.size()));

    for (auto& name : EventNames)
    {
        name->CompileTo(lines, state);
    }
}

void nemesis::animdata::AnimDataInfo::SerializeTo(DeqNstr& lines) const
{
    Name.SerializeTo(lines);
    UniqueCode.SerializeTo(lines);
    PlaybackSpeed.SerializeTo(lines);
    CropStartAmountLocalTime.SerializeTo(lines);
    CropEndAmountLocalTime.SerializeTo(lines);

    lines.emplace_back(std::to_string(EventNames.size()));

    for (auto& name : EventNames)
    {
        name->SerializeTo(lines);
    }
}

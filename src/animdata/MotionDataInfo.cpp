#include "animdata/MotionDataInfo.h"

#include "core/CompileState.h"

void nemesis::animdata::MotionDataInfo::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    UniqueCode.CompileTo(lines, state);
    Duration.CompileTo(lines, state);

    auto element_ptr = &lines.emplace_back("");
    size_t size      = lines.size();

    for each (auto& coordination in CoordinationData)
    {
        coordination->CompileTo(lines, state);
    }

    (*element_ptr) = std::to_string(lines.size() - size);
    element_ptr    = &lines.emplace_back("");
    size           = lines.size();

    for each (auto& rotation in RotationData)
    {
        rotation->CompileTo(lines, state);
    }

    lines.emplace_back(std::to_string(lines.size() - size));
}

void nemesis::animdata::MotionDataInfo::SerializeTo(DeqNstr& lines) const
{
    UniqueCode.SerializeTo(lines);
    Duration.SerializeTo(lines);

    auto element_ptr = &lines.emplace_back("");
    size_t size      = lines.size();

    for each (auto& coordination in CoordinationData)
    {
        coordination->SerializeTo(lines);
    }

    (*element_ptr) = std::to_string(lines.size() - size);
    element_ptr    = &lines.emplace_back("");
    size           = lines.size();

    for each (auto& rotation in RotationData)
    {
        rotation->SerializeTo(lines);
    }

    lines.emplace_back(std::to_string(lines.size() - size));
}

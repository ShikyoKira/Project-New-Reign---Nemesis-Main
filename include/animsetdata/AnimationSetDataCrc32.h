#pragma once

#include "core/NLine.h"
#include "core/CollectionObject.h"

namespace nemesis
{
    struct AnimationSetDataCrc32 : public nemesis::NObject
    {
    private:
        UPtr<nemesis::NLine> Directory;
        UPtr<nemesis::NLine> FileName;
        UPtr<nemesis::NLine> Extension;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        void MatchAndUpdate(nemesis::AnimationSetDataCrc32& crc32);
    };

    struct AnimationSetDataCrc32List : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationSetDataCrc32>> Crc32List;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        void AddAnimation(UPtr<nemesis::AnimationSetDataCrc32>&& crc32);
        void MatchAndUpdate(nemesis::AnimationSetDataCrc32List& crc32_list);
    };
}

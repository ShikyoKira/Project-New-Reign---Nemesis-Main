#pragma once

#include "core/NLine.h"
#include "core/LineStream.h"

namespace nemesis
{
    struct IfObject;
    struct ForEachObject;
    struct CollectionObject;

	struct AnimationSetDataState : public nemesis::NObject
    {
    private:
        enum DataType
        {
            EquipCounter,
            EquipList,
            TypeCounter,
            TypeList,
            AnimationCounter,
            AnimationList,
            Crc32Counter,
            Crc32List,
        };

        UPtr<nemesis::CollectionObject> StateData;
        static Vec<UPtr<nemesis::NObject>> ParseModObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager, bool& has_new_state);
        static UPtr<nemesis::ForEachObject> ParseForEachObject(nemesis::LineStream& stream,
                                                               nemesis::SemanticManager& manager,
                                                               bool& has_new_state);
        static UPtr<nemesis::IfObject>
        ParseIfObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager, bool& has_new_state);
    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        static Vec<UPtr<nemesis::NObject>> ParseObjects(nemesis::LineStream& stream,
                                                        nemesis::SemanticManager& manager,
                                                        Vec<nemesis::AnimationSetDataState*>& project_list);
    };
}

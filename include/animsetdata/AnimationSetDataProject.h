#pragma once

#include "core/NObject.h"

#include "animsetdata/AnimationSetDataEquip.h"
#include "animsetdata/AnimationSetDataType.h"
#include "animsetdata/AnimationSetDataAnimation.h"
#include "animsetdata/AnimationSetDataCrc32.h"

namespace nemesis
{
    struct ForEachObject;

	struct AnimationSetDataProject : public nemesis::NObject
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

        //UPtr<nemesis::AnimationSetDataEquipList> EquipList;
        //UPtr<nemesis::AnimationSetDataTypeList> TypeList;
        //UPtr<nemesis::AnimationSetDataAnimationList> AnimationList;
        //UPtr<nemesis::AnimationSetDataCrc32List> Crc32List;
        Vec<UPtr<nemesis::NObject>> Headers;
        UPtr<nemesis::CollectionObject> ProjectData;

        static bool IsProjectEnd(nemesis::LineStream& stream, bool& start);

        static Vec<UPtr<nemesis::NObject>>
        ParseModObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position);
        static Vec<UPtr<nemesis::NObject>> ParseModObjects(nemesis::LineStream& stream,
                                                           nemesis::SemanticManager& manager,
                                                           int start_position,
                                                           bool& has_new_project);
        static UPtr<nemesis::ForEachObject> ParseForEachObjects(nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager,
                                                                int start_position);
        static UPtr<nemesis::ForEachObject> ParseForEachObjects(nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager,
                                                                int start_position,
                                                                bool& has_new_project);
        static UPtr<nemesis::IfObject>
        ParseIfObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position);
        static UPtr<nemesis::IfObject> ParseIfObjects(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager,
                                                      int start_position,
                                                      bool& has_new_project);

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        static Deq<UPtr<nemesis::NObject>> ParseObjects(nemesis::LineStream& stream,
                                                        nemesis::SemanticManager& manager,
                                                        Deq<nemesis::AnimationSetDataProject*>& project_list);
    };
}

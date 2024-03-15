#pragma once

#include "core/NObject.h"

namespace nemesis
{
    struct AnimationSetDataType : public nemesis::NObject
    {
    private:
        UPtr<nemesis::NLine> Name;
        UPtr<nemesis::NLine> Type1;
        UPtr<nemesis::NLine> Type2;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataType> Clone() const;

        void MatchAndUpdate(nemesis::AnimationSetDataType& type);
    };

    struct AnimationSetDataTypeList : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationSetDataType>> TypeList;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataTypeList> Clone() const;

        void AddType(UPtr<nemesis::AnimationSetDataType>&& type);
        void MatchAndUpdate(nemesis::AnimationSetDataTypeList& type_list);
    };
}

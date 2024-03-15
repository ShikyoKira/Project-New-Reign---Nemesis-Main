#pragma once

#include "core/NLine.h"

namespace nemesis
{
    struct AnimationSetDataEquip : public nemesis::NLine
    {
    public:
        AnimationSetDataEquip() = default;
        AnimationSetDataEquip(const std::string& expression,
                              size_t linenum,
                              const std::filesystem::path& filepath,
                              const nemesis::SemanticManager& manager);
        AnimationSetDataEquip(const nemesis::Line& line, const nemesis::SemanticManager& manager);
    };

    struct AnimationSetDataEquipList : public nemesis::NObject
    {
    private:
        Vec<UPtr<nemesis::AnimationSetDataEquip>> EquipList;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataEquipList> Clone() const;

        void AddEquip(UPtr<nemesis::AnimationSetDataEquip>&& equip);
    };
}

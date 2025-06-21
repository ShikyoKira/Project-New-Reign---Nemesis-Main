#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbModifierList : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xa4180ca1,
                                                "hkbModifierList",
                                                &nemesis::hkbModifier::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbModifierList);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbModifier>> m_modifiers;

    public:
        hkbModifierList() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbModifierList::Class;
}

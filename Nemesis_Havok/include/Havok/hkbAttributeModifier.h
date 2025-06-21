#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbAttributeModifierAssignment.h"

namespace nemesis
{
    struct hkbAttributeModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x1245d97d,
                                                "hkbAttributeModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbAttributeModifier);

        nemesis::hkArray<nemesis::hkbAttributeModifierAssignment> m_assignments;

    public:
        hkbAttributeModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAttributeModifier::Class;
}

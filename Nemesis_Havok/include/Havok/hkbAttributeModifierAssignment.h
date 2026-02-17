#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbAttributeModifierAssignment : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x48b8ad52,
                                                "hkbAttributeModifierAssignment",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbAttributeModifierAssignment);

        int m_attributeIndex{};
        float m_attributeValue{};

    public:
        hkbAttributeModifierAssignment() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAttributeModifierAssignment::Class;
}

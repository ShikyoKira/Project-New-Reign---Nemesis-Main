#pragma once

#include "Havok/hkRefCountedProperties.h"

namespace nemesis
{
    struct hkpBreakableMaterial : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x71e403ca,
                                                "hkpBreakableMaterial",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        float m_strength;
        int m_typeAndFlags;
        nemesis::hkRefPtr<nemesis::hkRefCountedProperties> m_extractedMotion;

    public:
        hkpBreakableMaterial(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBreakableMaterial::Class;
}

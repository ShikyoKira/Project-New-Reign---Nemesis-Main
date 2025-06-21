#pragma once

#include "Havok/hkRefCountedPropertiesEntry.h"

namespace nemesis
{
    struct hkRefCountedProperties : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x7c574867,
                                                "hkRefCountedProperties",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkRefCountedProperties);

        nemesis::hkArray<nemesis::hkRefCountedPropertiesEntry> m_entries;

    public:
        hkRefCountedProperties() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRefCountedProperties::Class;
}

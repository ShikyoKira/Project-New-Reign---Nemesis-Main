#pragma once

#include "Havok/hkpPhysicsSystem.h"

#include "Havok/hkpSerializedAgentNnEntry.h"

namespace nemesis
{
    struct hkpPhysicsSystemWithContacts : nemesis::hkpPhysicsSystem
    {
        static constexpr nemesis::hkClass Class{0xd0fd4bbe,
                                                "hkpPhysicsSystemWithContacts",
                                                &nemesis::hkpPhysicsSystem::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPhysicsSystemWithContacts);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpSerializedAgentNnEntry>> m_contacts;

    public:
        hkpPhysicsSystemWithContacts() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPhysicsSystemWithContacts::Class;
}

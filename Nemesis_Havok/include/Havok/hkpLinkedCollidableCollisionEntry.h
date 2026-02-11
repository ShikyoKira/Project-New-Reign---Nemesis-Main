#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpAgentNnEntry;
    struct hkpLinkedCollidable;

    struct hkpLinkedCollidableCollisionEntry : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpLinkedCollidableCollisionEntry",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpLinkedCollidableCollisionEntry);

        nemesis::hkpAgentNnEntry* m_agentEntry = nullptr;
        nemesis::hkpLinkedCollidable* m_partner = nullptr;

    public:
        hkpLinkedCollidableCollisionEntry() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinkedCollidableCollisionEntry::Class;
}

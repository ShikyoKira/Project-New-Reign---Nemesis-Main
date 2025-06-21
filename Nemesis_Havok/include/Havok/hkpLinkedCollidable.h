#pragma once

#include "Havok/hkpCollidable.h"
#include "Havok/hkpLinkedCollidableCollisionEntry.h"

namespace nemesis
{
    struct hkpLinkedCollidable : nemesis::hkpCollidable
    {
        static constexpr nemesis::hkClass Class{0xe1a81497,
                                                "hkpLinkedCollidable",
                                                &nemesis::hkpCollidable::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLinkedCollidable);

        nemesis::hkArray<nemesis::hkpLinkedCollidableCollisionEntry> m_collisionEntries;

    public:
        hkpLinkedCollidable() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinkedCollidable::Class;
}

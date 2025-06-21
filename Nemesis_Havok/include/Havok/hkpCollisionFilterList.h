#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

namespace nemesis
{
    struct hkpCollisionFilterList : nemesis::hkpCollisionFilter
    {
        static constexpr nemesis::hkClass Class{0x2603bf04,
                                                "hkpCollisionFilterList",
                                                &nemesis::hkpCollisionFilter::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCollisionFilterList);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpCollisionFilter>> m_collisionFilters;

    public:
        hkpCollisionFilterList() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCollisionFilterList::Class;
}

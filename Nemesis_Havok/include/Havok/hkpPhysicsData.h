#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpWorldCinfo.h"
#include "Havok/hkpPhysicsSystem.h"

namespace nemesis
{
    struct hkpPhysicsData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc2a461e4,
                                                "hkpPhysicsData",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPhysicsData);

        nemesis::hkRefPtr<nemesis::hkpWorldCinfo> m_worldCinfo;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpPhysicsSystem>> m_systems;

    public:
        hkpPhysicsData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPhysicsData::Class;
}

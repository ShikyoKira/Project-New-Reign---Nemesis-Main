#pragma once

#include "Havok/hkAabb.h"

#include "Havok/Base/hkpPhantom.h"

namespace nemesis
{
    struct hkpAabbPhantom : nemesis::hkpPhantom
    {
        static constexpr nemesis::hkClass Class{0x2c5189dd,
                                                "hkpAabbPhantom",
                                                &nemesis::hkpPhantom::Class,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpAabbPhantom);

        nemesis::hkAabb m_aabb;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_overlappingCollidables;
        bool m_orderDirty{};

    public:
        hkpAabbPhantom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAabbPhantom::Class;
}

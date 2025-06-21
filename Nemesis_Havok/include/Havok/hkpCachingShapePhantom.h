#pragma once

#include "Havok/Base/hkpShapePhantom.h"

namespace nemesis
{
    struct hkpCachingShapePhantom : nemesis::hkpShapePhantom
    {
        static constexpr nemesis::hkClass Class{0xcf227f58,
                                                "hkpCachingShapePhantom",
                                                &nemesis::hkpShapePhantom::Class,
                                                448,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCachingShapePhantom);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_collisionDetails;
        bool m_orderDirty;

    public:
        hkpCachingShapePhantom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCachingShapePhantom::Class;
}

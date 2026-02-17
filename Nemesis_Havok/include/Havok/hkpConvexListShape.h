#pragma once

#include "Havok/Base/hkpConvexShape.h"
#include "Havok/Base/hkpShapeContainer.h"

namespace nemesis
{
    struct hkpConvexListShape : nemesis::hkpConvexShape, nemesis::hkpShapeContainer
    {
        static constexpr nemesis::hkClass Class{0x450b26e8,
                                                "hkpConvexListShape",
                                                &nemesis::hkpConvexShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexListShape);

        float m_minDistanceToUseConvexHullForGetClosestPoints{};
        nemesis::hkVector4 m_aabbHalfExtents;
        nemesis::hkVector4 m_aabbCenter;
        bool m_useCachedAabb{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConvexShape>> m_childShapes;

    public:
        hkpConvexListShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexListShape::Class;
}

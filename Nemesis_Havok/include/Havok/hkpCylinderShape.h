#pragma once

#include "Havok/Base/hkpConvexShape.h"

namespace nemesis
{
    struct hkpCylinderShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0x3e463c3a,
                                                "hkpCylinderShape",
                                                &nemesis::hkpConvexShape::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCylinderShape);

        float m_cylRadius{};
        float m_cylBaseRadiusFactorForHeightFieldCollisions{};
        nemesis::hkVector4 m_vertexA;
        nemesis::hkVector4 m_vertexB;
        nemesis::hkVector4 m_perpendicular1;
        nemesis::hkVector4 m_perpendicular2;

    public:
        hkpCylinderShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCylinderShape::Class;
}

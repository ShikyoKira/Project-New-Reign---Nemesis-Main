#pragma once

#include "Havok/Base/hkpConvexShape.h"

namespace nemesis
{
    struct hkpTriangleShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0x95ad1a25,
                                                "hkpTriangleShape",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpTriangleShape);

        using WeldingType = nemesis::hkInternalClassEnumItem<unsigned char>;

        unsigned short m_weldingInfo;
        nemesis::hkEnum<WeldingType, unsigned char> m_weldingType = {
            {"WELDING_TYPE_ANTICLOCKWISE", 0},
            {"WELDING_TYPE_CLOCKWISE", 4},
            {"WELDING_TYPE_TWO_SIDED", 5},
            {"WELDING_TYPE_NONE", 6},
        };
        unsigned char m_isExtruded;
        nemesis::hkVector4 m_vertexA;
        nemesis::hkVector4 m_vertexB;
        nemesis::hkVector4 m_vertexC;
        nemesis::hkVector4 m_extrusion;

    public:
        hkpTriangleShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTriangleShape::Class;
}

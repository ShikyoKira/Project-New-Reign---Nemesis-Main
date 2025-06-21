#pragma once

#include "Havok/hkpMeshShapeSubpart.h"
#include "Havok/hkpShapeCollection.h"

namespace nemesis
{
    struct hkpMeshShape : nemesis::hkpShapeCollection
    {
        static constexpr nemesis::hkClass Class{0x3bf12c0f,
                                                "hkpMeshShape",
                                                &nemesis::hkpShapeCollection::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMeshShape);

        using WeldingType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkVector4 m_scaling;
        int m_numBitsForSubpartIndex;
        nemesis::hkArray<nemesis::hkpMeshShapeSubpart> m_subparts;
        nemesis::hkArray<unsigned short> m_weldingInfo;
        nemesis::hkEnum<WeldingType, unsigned char> m_weldingType = {
            {"WELDING_TYPE_ANTICLOCKWISE", 0},
            {"WELDING_TYPE_CLOCKWISE", 4},
            {"WELDING_TYPE_TWO_SIDED", 5},
            {"WELDING_TYPE_NONE", 6},
        };
        float m_radius;
        std::array<int, 3> m_pad;

    public:
        hkpMeshShape() noexcept;
        hkpMeshShape(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMeshShape::Class;
}

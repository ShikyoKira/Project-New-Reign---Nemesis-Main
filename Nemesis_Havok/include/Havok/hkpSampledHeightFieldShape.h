#pragma once

#include "Havok/Base/hkpHeightFieldShape.h"

#include "Havok/hkpSampledHeightFieldShapeCoarseMinMaxLevel.h"

namespace nemesis
{
    struct hkpSampledHeightFieldShape : nemesis::hkpHeightFieldShape
    {
        static constexpr nemesis::hkClass Class{0x11213421,
                                                "hkpSampledHeightFieldShape",
                                                &nemesis::hkpHeightFieldShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSampledHeightFieldShape);

        using HeightFieldType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkArray<nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel> m_coarseTreeData;
        int m_coarseness{};
        float m_raycastMinY{};
        float m_raycastMaxY{};
        int m_xRes{};
        int m_zRes{};
        float m_heightCenter{};
        bool m_useProjectionBasedHeight{};
        nemesis::hkEnum<HeightFieldType, unsigned char> m_heightfieldType = {
            {"HEIGHTFIELD_STORAGE", 0},
            {"HEIGHTFIELD_COMPRESSED", 1},
            {"HEIGHTFIELD_USER", 2},
            {"HEIGHTFIELD_MAX_ID", 3},
        };
        nemesis::hkVector4 m_intToFloatScale;
        nemesis::hkVector4 m_floatToIntScale;
        nemesis::hkVector4 m_floatToIntOffsetFloorCorrected;
        nemesis::hkVector4 m_extents;

    public:
        hkpSampledHeightFieldShape() noexcept;
        hkpSampledHeightFieldShape(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSampledHeightFieldShape::Class;
}

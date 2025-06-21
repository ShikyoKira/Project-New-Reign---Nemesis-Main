#pragma once

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpBvTreeShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0xa823d623,
                                                "hkpBvTreeShape",
                                                &nemesis::hkpShape::Class,
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
        using BvTreeType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<BvTreeType, unsigned char> m_bvTreeType_0 = {
            {"BVTREE_MOPP", 0},
            {"BVTREE_TRISAMPLED_HEIGHTFIELD", 1},
            {"BVTREE_USER", 2},
            {"BVTREE_MAX", 3},
        };
        nemesis::hkEnum<BvTreeType, unsigned char> m_bvTreeType_1 = {
            {"BVTREE_MOPP", 0},
            {"BVTREE_TRISAMPLED_HEIGHTFIELD", 1},
            {"BVTREE_STATIC_COMPOUND", 2},
            {"BVTREE_COMPRESSED_MESH", 3},
            {"BVTREE_USER", 4},
            {"BVTREE_MAX", 5},
        };

    public:
        hkpBvTreeShape(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBvTreeShape::Class;
}

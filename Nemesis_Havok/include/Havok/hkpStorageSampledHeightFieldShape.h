#pragma once

#include "Havok/hkpSampledHeightFieldShape.h"

namespace nemesis
{
    struct hkpStorageSampledHeightFieldShape : nemesis::hkpSampledHeightFieldShape
    {
        static constexpr nemesis::hkClass Class{0x15ff414b,
                                                "hkpStorageSampledHeightFieldShape",
                                                &nemesis::hkpSampledHeightFieldShape::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpStorageSampledHeightFieldShape);

        nemesis::hkArray<float> m_storage;
        bool m_triangleFlip;

    public:
        hkpStorageSampledHeightFieldShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStorageSampledHeightFieldShape::Class;
}

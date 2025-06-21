#pragma once

#include "Havok/hkpSampledHeightFieldShape.h"

namespace nemesis
{
    struct hkpCompressedSampledHeightFieldShape : nemesis::hkpSampledHeightFieldShape
    {
        static constexpr nemesis::hkClass Class{0x97b6e143,
                                                "hkpCompressedSampledHeightFieldShape",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCompressedSampledHeightFieldShape);

        nemesis::hkArray<unsigned short> m_storage;
        bool m_triangleFlip;
        float m_offset;
        float m_scale;

    public:
        hkpCompressedSampledHeightFieldShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCompressedSampledHeightFieldShape::Class;
}

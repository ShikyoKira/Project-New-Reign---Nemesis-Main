#pragma once

#include "Havok/Base/hkaAnimation.h"

namespace nemesis
{
    struct hkaSplineCompressedAnimation : nemesis::hkaAnimation
    {
        static constexpr nemesis::hkClass Class{0x792ee0bb,
                                                "hkaSplineCompressedAnimation",
                                                &nemesis::hkaAnimation::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSplineCompressedAnimation);

        int m_numFrames{};
        int m_numBlocks{};
        int m_maxFramesPerBlock{};
        int m_maskAndQuantizationSize{};
        float m_blockDuration{};
        float m_blockInverseDuration{};
        float m_frameDuration{};
        nemesis::hkArray<unsigned int> m_blockOffsets;
        nemesis::hkArray<unsigned int> m_floatBlockOffsets;
        nemesis::hkArray<unsigned int> m_transformOffsets;
        nemesis::hkArray<unsigned int> m_floatOffsets;
        nemesis::hkArray<unsigned char> m_data;
        int m_endian{};

    public:
        hkaSplineCompressedAnimation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSplineCompressedAnimation::Class;
}

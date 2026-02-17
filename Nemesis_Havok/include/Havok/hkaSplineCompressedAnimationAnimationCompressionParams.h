#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSplineCompressedAnimationAnimationCompressionParams : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xde830789,
                                                "hkaSplineCompressedAnimationAnimationCompressionParams",
                                                nullptr,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSplineCompressedAnimationAnimationCompressionParams);

        unsigned short m_maxFramesPerBlock{};
        bool m_enableSampleSingleTracks{};

    public:
        hkaSplineCompressedAnimationAnimationCompressionParams() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSplineCompressedAnimationAnimationCompressionParams::Class;
}

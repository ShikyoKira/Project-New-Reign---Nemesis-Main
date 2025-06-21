#pragma once

#include "Havok/hkaSplineCompressedAnimationTrackCompressionParams.h"

namespace nemesis
{
    struct hkaSplineCompressedAnimationPerTrackCompressionParams : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkaSplineCompressedAnimationPerTrackCompressionParams",
                                                nullptr,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSplineCompressedAnimationPerTrackCompressionParams);

        nemesis::hkArray<nemesis::hkaSplineCompressedAnimationTrackCompressionParams> m_parameterPalette;
        nemesis::hkArray<int> m_trackIndexToPaletteIndex;
        nemesis::hkArray<int> m_floatTrackIndexToPaletteIndex;

    public:
        hkaSplineCompressedAnimationPerTrackCompressionParams() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSplineCompressedAnimationPerTrackCompressionParams::Class;
}

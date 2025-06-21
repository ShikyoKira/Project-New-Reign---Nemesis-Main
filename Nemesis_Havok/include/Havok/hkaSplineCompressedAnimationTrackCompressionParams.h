#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSplineCompressedAnimationTrackCompressionParams : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x42e878d3,
                                                "hkaSplineCompressedAnimationTrackCompressionParams",
                                                nullptr,
                                                28,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSplineCompressedAnimationTrackCompressionParams);

        using RotationQuantization = nemesis::hkInternalClassEnumItem<unsigned char>;
        using ScalarQuantization = nemesis::hkInternalClassEnumItem<unsigned char>;

#define ROTATION_QUANTIZATION \
    {                         \
        {"POLAR32", 0},       \
        {"THREECOMP40", 1},   \
        {"THREECOMP48", 2},   \
        {"THREECOMP24", 3},   \
        {"STRAIGHT16", 4},    \
        {"UNCOMPRESSED", 5},  \
    };

#define SCLAR_QUANTIZATION \
    {                      \
        {"BITS8", 0},      \
        {"BITS16", 1},     \
    };

        float m_rotationTolerance;
        float m_translationTolerance;
        float m_scaleTolerance;
        float m_floatingTolerance;
        unsigned short m_rotationDegree;
        unsigned short m_translationDegree;
        unsigned short m_scaleDegree;
        unsigned short m_floatingDegree;
        nemesis::hkEnum<RotationQuantization, unsigned char> m_rotationQuantizationType
            = ROTATION_QUANTIZATION;
        nemesis::hkEnum<ScalarQuantization, unsigned char> m_translationQuantizationType = SCLAR_QUANTIZATION;
        nemesis::hkEnum<ScalarQuantization, unsigned char> m_scaleQuantizationType       = SCLAR_QUANTIZATION;
        nemesis::hkEnum<ScalarQuantization, unsigned char> m_floatQuantizationType       = SCLAR_QUANTIZATION;

    public:
        hkaSplineCompressedAnimationTrackCompressionParams() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSplineCompressedAnimationTrackCompressionParams::Class;
}

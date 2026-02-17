#pragma once

#include "Havok/Base/hkaAnimatedReferenceFrame.h"

#include "Havok/hkaAnnotationTrack.h"

namespace nemesis
{
    struct hkaAnimation : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa6fa7e88,
                                                "hkaAnimation",
                                                &nemesis::hkReferencedObject::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using AnimationType = nemesis::hkInternalClassEnumItem<int>;

        nemesis::hkEnum<AnimationType, int> m_type_0 = {
            {"HK_UNKNOWN_ANIMATION", 0},
            {"HK_INTERLEAVED_ANIMATION", 1},
            {"HK_DELTA_COMPRESSED_ANIMATION", 2},
            {"HK_WAVELET_COMPRESSED_ANIMATION", 3},
            {"HK_MIRRORED_ANIMATION", 4},
            {"HK_SPLINE_COMPRESSED_ANIMATION", 5},
            {"HK_QUANTIZED_COMPRESSED_ANIMATION", 6},
        };
        nemesis::hkEnum<AnimationType, int> m_type_1 = {
            {"HK_UNKNOWN_ANIMATION", 0},
            {"HK_INTERLEAVED_ANIMATION", 1},
            {"HK_MIRRORED_ANIMATION", 2},
            {"HK_SPLINE_COMPRESSED_ANIMATION", 3},
            {"HK_QUANTIZED_COMPRESSED_ANIMATION", 4},
            {"HK_PREDICTIVE_COMPRESSED_ANIMATION", 5},
            {"HK_REFERENCE_POSE_ANIMATION", 6},
        };
        float m_duration{};
        int m_numberOfTransformTracks{};
        int m_numberOfFloatTracks{};
        nemesis::hkRefPtr<nemesis::hkaAnimatedReferenceFrame> m_extractedMotion;
        nemesis::hkRefPtr<nemesis::hkaAnnotationTrack> m_annotationTracks_0;
        int m_numAnnotationTracks{};
        nemesis::hkArray<nemesis::hkaAnnotationTrack> m_annotationTracks_1;

    public:
        hkaAnimation(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaAnimation::Class;
}

#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaQuantizedAnimationTrackCompressionParams : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf7d64649,
                                                "hkaQuantizedAnimationTrackCompressionParams",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaQuantizedAnimationTrackCompressionParams);

        float m_rotationTolerance{};
        float m_translationTolerance{};
        float m_scaleTolerance{};
        float m_floatingTolerance{};

    public:
        hkaQuantizedAnimationTrackCompressionParams() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaQuantizedAnimationTrackCompressionParams::Class;
}

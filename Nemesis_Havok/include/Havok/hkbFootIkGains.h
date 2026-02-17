#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbFootIkGains : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa681b7f0,
                                                "hkbFootIkGains",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkGains);

        float m_onOffGain{};
        float m_groundAscendingGain{};
        float m_groundDescendingGain{};
        float m_footPlantedGain{};
        float m_footRaisedGain{};
        float m_footUnlockGain{};
        float m_worldFromModelFeedbackGain{};
        float m_errorUpDownBias{};
        float m_alignWorldFromModelGain{};
        float m_hipOrientationGain{};
        float m_maxKneeAngleDifference{};
        float m_ankleOrientationGain{};

    public:
        hkbFootIkGains() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkGains::Class;
}

#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbTimerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x338b4879,
                                                "hkbTimerModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbTimerModifier);

        float m_alarmTimeSeconds;
        nemesis::hkbEventProperty m_alarmEvent;
        float m_secondsElapsed;

    public:
        hkbTimerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTimerModifier::Class;
}

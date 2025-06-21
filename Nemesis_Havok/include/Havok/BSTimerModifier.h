#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSTimerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x531f3292,
                                                "BSTimerModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(BSTimerModifier);

        float m_alarmTimeSeconds;
        nemesis::hkbEventProperty m_alarmEvent;
        bool m_resetAlarm;
        float m_secondsElapsed;

    public:
        BSTimerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSTimerModifier::Class;
}

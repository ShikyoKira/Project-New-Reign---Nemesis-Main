#pragma once

#include "Havok/Base/hkbModifierWrapper.h"

namespace nemesis
{
    struct hkbDelayedModifier : nemesis::hkbModifierWrapper
    {
        static constexpr nemesis::hkClass Class{0x8e101a7a,
                                                "hkbDelayedModifier",
                                                &nemesis::hkbModifierWrapper::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbDelayedModifier);

        float m_delaySeconds;
        float m_durationSeconds;
        float m_secondsElapsed;
        bool m_isActive;

    public:
        hkbDelayedModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbDelayedModifier::Class;
}

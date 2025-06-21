#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbCameraShakeEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0x64136982,
                                                "hkbCameraShakeEventPayload",
                                                &nemesis::hkbEventPayload::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCameraShakeEventPayload);

        float m_amplitude;
        float m_halfLife;

    public:
        hkbCameraShakeEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCameraShakeEventPayload::Class;
}

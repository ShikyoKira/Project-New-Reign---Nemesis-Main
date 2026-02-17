#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbPoweredRagdollControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf5ba21b,
                                                "hkbPoweredRagdollControlData",
                                                nullptr,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbPoweredRagdollControlData);

        float m_maxForce{}; // align 16
        float m_tau{};
        float m_damping{};
        float m_proportionalRecoveryVelocity{};
        float m_constantRecoveryVelocity{};

    public:
        hkbPoweredRagdollControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbPoweredRagdollControlData::Class;
}

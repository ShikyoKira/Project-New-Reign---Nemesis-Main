#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpSolverInfoDeactivationInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpSolverInfoDeactivationInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSolverInfoDeactivationInfo);

        float m_linearVelocityThresholdInv;
        float m_angularVelocityThresholdInv;
        float m_slowObjectVelocityMultiplier;
        float m_relativeSleepVelocityThreshold;
        std::array<float, 2> m_maxDistSqrd;
        std::array<nemesis::hkHalf, 2> m_maxRotSqrd_0;
        std::array<float, 2> m_maxRotSqrd_1;

    public:
        hkpSolverInfoDeactivationInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSolverInfoDeactivationInfo::Class;
}

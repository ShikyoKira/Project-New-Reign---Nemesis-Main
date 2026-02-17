#pragma once

#include "Havok/hkpSolverInfoDeactivationInfo.h"

namespace nemesis
{
    struct hkpSolverInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpSolverInfo",
                                                nullptr,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSolverInfo);

        float m_one{}; // align 16
        float m_tau{};
        float m_damping{};
        float m_frictionTau{};
        nemesis::hkVector4 m_globalAccelerationPerSubStep;
        nemesis::hkVector4 m_globalAccelerationPerStep;
        nemesis::hkVector4 m_integrateVelocityFactor;
        nemesis::hkVector4 m_invIntegrateVelocityFactor;
        float m_dampDivTau{};
        float m_tauDivDamp{};
        float m_dampDivFrictionTau{};
        float m_frictionTauDivDamp{};
        float m_contactRestingVelocity{};
        std::array<nemesis::hkpSolverInfoDeactivationInfo, 6> m_deactivationInfo;
        float m_deltaTime{};
        float m_invDeltaTime{};
        int m_numSteps{};
        int m_numMicroSteps{};
        float m_invNumMicroSteps{};
        float m_invNumSteps{};
        bool m_forceCoherentConstraintOrderingInSolver{};
        std::array<unsigned char, 2> m_deactivationNumInactiveFramesSelectFlag{};
        unsigned char m_deactivationIntegrateCounter{};
        float m_maxConstraintViolationSqrd{};

    public:
        hkpSolverInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSolverInfo::Class;
}

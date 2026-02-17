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

        float m_one         = 0.0f; // align 16
        float m_tau         = 0.0f;
        float m_damping     = 0.0f;
        float m_frictionTau{};
        nemesis::hkVector4 m_globalAccelerationPerSubStep;
        nemesis::hkVector4 m_globalAccelerationPerStep;
        nemesis::hkVector4 m_integrateVelocityFactor;
        nemesis::hkVector4 m_invIntegrateVelocityFactor;
        float m_dampDivTau             = 0.0f;
        float m_tauDivDamp             = 0.0f;
        float m_dampDivFrictionTau     = 0.0f;
        float m_frictionTauDivDamp     = 0.0f;
        float m_contactRestingVelocity{};
        std::array<nemesis::hkpSolverInfoDeactivationInfo, 6> m_deactivationInfo;
        float m_deltaTime        = 0.0f;
        float m_invDeltaTime     = 0.0f;
        int m_numSteps           = 0;
        int m_numMicroSteps      = 0;
        float m_invNumMicroSteps{};
        float m_invNumSteps      = 0.0f;
        bool m_forceCoherentConstraintOrderingInSolver{};
        std::array<unsigned char, 2> m_deactivationNumInactiveFramesSelectFlag{};
        unsigned char m_deactivationIntegrateCounter = 0;
        float m_maxConstraintViolationSqrd           = 0.0f;

    public:
        hkpSolverInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSolverInfo::Class;
}

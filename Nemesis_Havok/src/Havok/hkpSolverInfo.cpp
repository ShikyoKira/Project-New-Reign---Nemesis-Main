#include "Havok/hkpSolverInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSolverInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSolverInfo::Class; });

nemesis::hkpSolverInfo::hkpSolverInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSolverInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSolverInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue(serializer.GetContentsVersion() >= HavokVersion::HK_2011_2_0 ? "padding" : "one",
                          m_one);                        // offset: 0/0 size: 4/4 align: 16/16
    serializer.WriteValue("tau", m_tau);                 // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);         // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("frictionTau", m_frictionTau); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("globalAccelerationPerSubStep",
                          m_globalAccelerationPerSubStep); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("globalAccelerationPerStep",
                          m_globalAccelerationPerStep); // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("integrateVelocityFactor",
                          m_integrateVelocityFactor); // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("invIntegrateVelocityFactor",
                          m_invIntegrateVelocityFactor); // offset: 64/64 size: 16/16 align: 16/16
    serializer.WriteValue("dampDivTau", m_dampDivTau);   // offset: 80/80 size: 4/4 align: 4/4
    serializer.WriteValue("tauDivDamp", m_tauDivDamp);   // offset: 84/84 size: 4/4 align: 4/4
    serializer.WriteValue("dampDivFrictionTau", m_dampDivFrictionTau); // offset: 88/88 size: 4/4 align: 4/4
    serializer.WriteValue("frictionTauDivDamp", m_frictionTauDivDamp); // offset: 92/92 size: 4/4 align: 4/4
    serializer.WriteValue("contactRestingVelocity",
                          m_contactRestingVelocity); // offset: 96/96 size: 4/4 align: 4/4
    serializer.WriteObject("deactivationInfo",
                           m_deactivationInfo);                    // offset: 100/100 size: 168/168 align: 4/4
    serializer.WriteValue("deltaTime", m_deltaTime);               // offset: 268/268 size: 4/4 align: 4/4
    serializer.WriteValue("invDeltaTime", m_invDeltaTime);         // offset: 272/272 size: 4/4 align: 4/4
    serializer.WriteValue("numSteps", m_numSteps);                 // offset: 276/276 size: 4/4 align: 4/4
    serializer.WriteValue("numMicroSteps", m_numMicroSteps);       // offset: 280/280 size: 4/4 align: 4/4
    serializer.WriteValue("invNumMicroSteps", m_invNumMicroSteps); // offset: 284/284 size: 4/4 align: 4/4
    serializer.WriteValue("invNumSteps", m_invNumSteps);           // offset: 288/288 size: 4/4 align: 4/4
    serializer.WriteValue("forceCoherentConstraintOrderingInSolver",
                          m_forceCoherentConstraintOrderingInSolver); // offset: 292/292 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationNumInactiveFramesSelectFlag",
                          m_deactivationNumInactiveFramesSelectFlag); // offset: 293/293 size: 2/2 align: 1/1
    serializer.WriteValue("deactivationIntegrateCounter",
                          m_deactivationIntegrateCounter); // offset: 295/295 size: 1/1 align: 1/1
    serializer.WriteValue("maxConstraintViolationSqrd",
                          m_maxConstraintViolationSqrd); // offset: 296/296 size: 4/4 align: 4/4
    serializer.Skip(4);                                  // offset: 300/300 size: 4/4
    // class size: 304/304 align: 16/16
}

void nemesis::hkpSolverInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue(deserializer.GetContentsVersion() >= HavokVersion::HK_2011_2_0 ? "padding" : "one",
                           m_one);                        // offset: 0/0 size: 4/4 align: 16/16
    deserializer.ReadValue("tau", m_tau);                 // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);         // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("frictionTau", m_frictionTau); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("globalAccelerationPerSubStep",
                           m_globalAccelerationPerSubStep); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("globalAccelerationPerStep",
                           m_globalAccelerationPerStep); // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("integrateVelocityFactor",
                           m_integrateVelocityFactor); // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("invIntegrateVelocityFactor",
                           m_invIntegrateVelocityFactor); // offset: 64/64 size: 16/16 align: 16/16
    deserializer.ReadValue("dampDivTau", m_dampDivTau);   // offset: 80/80 size: 4/4 align: 4/4
    deserializer.ReadValue("tauDivDamp", m_tauDivDamp);   // offset: 84/84 size: 4/4 align: 4/4
    deserializer.ReadValue("dampDivFrictionTau", m_dampDivFrictionTau); // offset: 88/88 size: 4/4 align: 4/4
    deserializer.ReadValue("frictionTauDivDamp", m_frictionTauDivDamp); // offset: 92/92 size: 4/4 align: 4/4
    deserializer.ReadValue("contactRestingVelocity",
                           m_contactRestingVelocity); // offset: 96/96 size: 4/4 align: 4/4
    deserializer.ReadObject("deactivationInfo",
                            m_deactivationInfo);              // offset: 100/100 size: 168/168 align: 4/4
    deserializer.ReadValue("deltaTime", m_deltaTime);         // offset: 268/268 size: 4/4 align: 4/4
    deserializer.ReadValue("invDeltaTime", m_invDeltaTime);   // offset: 272/272 size: 4/4 align: 4/4
    deserializer.ReadValue("numSteps", m_numSteps);           // offset: 276/276 size: 4/4 align: 4/4
    deserializer.ReadValue("numMicroSteps", m_numMicroSteps); // offset: 280/280 size: 4/4 align: 4/4
    deserializer.ReadValue("invNumMicroSteps", m_invNumMicroSteps); // offset: 284/284 size: 4/4 align: 4/4
    deserializer.ReadValue("invNumSteps", m_invNumSteps);           // offset: 288/288 size: 4/4 align: 4/4
    deserializer.ReadValue("forceCoherentConstraintOrderingInSolver",
                           m_forceCoherentConstraintOrderingInSolver); // offset: 292/292 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationNumInactiveFramesSelectFlag",
                           m_deactivationNumInactiveFramesSelectFlag); // offset: 293/293 size: 2/2 align: 1/1
    deserializer.ReadValue("deactivationIntegrateCounter",
                           m_deactivationIntegrateCounter); // offset: 295/295 size: 1/1 align: 1/1
    deserializer.ReadValue("maxConstraintViolationSqrd",
                           m_maxConstraintViolationSqrd); // offset: 296/296 size: 4/4 align: 4/4
    deserializer.Skip(4);                                 // offset: 300/300 size: 4/4
    // class size: 304/304 align: 16/16
}

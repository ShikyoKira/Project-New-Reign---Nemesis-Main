#include "Havok/hkbPoweredRagdollControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbPoweredRagdollControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbPoweredRagdollControlData::Class; });

nemesis::hkbPoweredRagdollControlData::hkbPoweredRagdollControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbPoweredRagdollControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbPoweredRagdollControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("maxForce", m_maxForce); // offset: 0/0 size: 4/4 align: 16/16
    serializer.WriteValue("tau", m_tau);           // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);   // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("proportionalRecoveryVelocity",
                          m_proportionalRecoveryVelocity); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("constantRecoveryVelocity",
                          m_constantRecoveryVelocity); // offset: 16/16 size: 4/4 align: 4/4
    serializer.Skip(12);                               // offset: 20/20 size: 12/12
    // class size: 32/32 align: 16/16
}

void nemesis::hkbPoweredRagdollControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("maxForce", m_maxForce); // offset: 0/0 size: 4/4 align: 16/16
    deserializer.ReadValue("tau", m_tau);           // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);   // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("proportionalRecoveryVelocity",
                           m_proportionalRecoveryVelocity); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("constantRecoveryVelocity",
                           m_constantRecoveryVelocity); // offset: 16/16 size: 4/4 align: 4/4
    deserializer.Skip(12);                              // offset: 20/20 size: 12/12
    // class size: 32/32 align: 16/16
}

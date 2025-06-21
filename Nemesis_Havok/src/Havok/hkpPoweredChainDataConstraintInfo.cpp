#include "Havok/hkpPoweredChainDataConstraintInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPoweredChainDataConstraintInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPoweredChainDataConstraintInfo::Class; });

nemesis::hkpPoweredChainDataConstraintInfo::hkpPoweredChainDataConstraintInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPoweredChainDataConstraintInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPoweredChainDataConstraintInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("pivotInA", m_pivotInA);         // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("pivotInB", m_pivotInB);         // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("aTc", m_aTc);                   // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("bTc", m_bTc);                   // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteObject("motors", m_motors);            // offset: 64/64 size: 12/24 align: 4/8
    serializer.WriteValue("switchBodies", m_switchBodies); // offset: 76/88 size: 1/1 align: 1/1
    serializer.Pad(16);                                    // offset: 77/89 size: 3/7
    // class size: 80/96 align: 16/16
}

void nemesis::hkpPoweredChainDataConstraintInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("pivotInA", m_pivotInA);         // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("pivotInB", m_pivotInB);         // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("aTc", m_aTc);                   // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("bTc", m_bTc);                   // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadObject("motors", m_motors);            // offset: 64/64 size: 12/24 align: 4/8
    deserializer.ReadValue("switchBodies", m_switchBodies); // offset: 76/88 size: 1/1 align: 1/1
    deserializer.Pad(16);                                   // offset: 77/89 size: 3/7
    // class size: 80/96 align: 16/16
}

#include "Havok/hkpWorldDynamicsStepInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWorldDynamicsStepInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWorldDynamicsStepInfo::Class; });

nemesis::hkpWorldDynamicsStepInfo::hkpWorldDynamicsStepInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpWorldDynamicsStepInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWorldDynamicsStepInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("stepInfo", m_stepInfo);     // offset: 0/0 size: 16/16 align: 4/4
    serializer.WriteObject("solverInfo", m_solverInfo); // offset: 16/16 size: 304/304 align: 16/16
    // class size: 320/320 align: 16/16
}

void nemesis::hkpWorldDynamicsStepInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("stepInfo", m_stepInfo);     // offset: 0/0 size: 16/16 align: 4/4
    deserializer.ReadObject("solverInfo", m_solverInfo); // offset: 16/16 size: 304/304 align: 16/16
    // class size: 320/320 align: 16/16
}

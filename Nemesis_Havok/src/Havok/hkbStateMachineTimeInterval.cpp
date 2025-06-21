#include "Havok/hkbStateMachineTimeInterval.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineTimeInterval,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineTimeInterval::Class; });

nemesis::hkbStateMachineTimeInterval::hkbStateMachineTimeInterval() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineTimeInterval::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineTimeInterval::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("enterEventId", m_enterEventId); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("exitEventId", m_exitEventId);   // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("enterTime", m_enterTime);       // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("exitTime", m_exitTime);         // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkbStateMachineTimeInterval::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("enterEventId", m_enterEventId); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("exitEventId", m_exitEventId);   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("enterTime", m_enterTime);       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("exitTime", m_exitTime);         // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

#include "Havok/hkbStateMachineEventPropertyArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineEventPropertyArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineEventPropertyArray::Class; });

nemesis::hkbStateMachineEventPropertyArray::hkbStateMachineEventPropertyArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbStateMachineEventPropertyArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineEventPropertyArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("events", m_events);           // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbStateMachineEventPropertyArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("events", m_events);                // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

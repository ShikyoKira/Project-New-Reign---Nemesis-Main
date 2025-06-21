#include "Havok/hkbStateMachineTransitionInfoArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineTransitionInfoArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineTransitionInfoArray::Class; });

nemesis::hkbStateMachineTransitionInfoArray::hkbStateMachineTransitionInfoArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbStateMachineTransitionInfoArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineTransitionInfoArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("transitions", m_transitions); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbStateMachineTransitionInfoArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("transitions", m_transitions);      // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

#include "Havok/hkbBehaviorGraphInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorGraphInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorGraphInternalState::Class; });

nemesis::hkbBehaviorGraphInternalState::hkbBehaviorGraphInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBehaviorGraphInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorGraphInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("nodeInternalStateInfos",
                           m_nodeInternalStateInfos);               // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("variableValueSet", m_variableValueSet); // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkbBehaviorGraphInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("nodeInternalStateInfos",
                            m_nodeInternalStateInfos);               // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("variableValueSet", m_variableValueSet); // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}

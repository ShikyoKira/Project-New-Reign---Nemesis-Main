#include "Havok/hkbBehaviorGraphInternalStateInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorGraphInternalStateInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorGraphInternalStateInfo::Class; });

nemesis::hkbBehaviorGraphInternalStateInfo::hkbBehaviorGraphInternalStateInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBehaviorGraphInternalStateInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorGraphInternalStateInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);              // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteObject("internalState", m_internalState);         // offset: 16/24 size: 4/8 align: 4/8
    serializer.WriteObject("auxiliaryNodeInfo", m_auxiliaryNodeInfo); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("activeEventIds", m_activeEventIds);       // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("activeVariableIds", m_activeVariableIds); // offset: 44/64 size: 12/16 align: 4/8
    // class size: 56/80 align: 8/8
}

void nemesis::hkbBehaviorGraphInternalStateInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);              // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadObject("internalState", m_internalState);         // offset: 16/24 size: 4/8 align: 4/8
    deserializer.ReadObject("auxiliaryNodeInfo", m_auxiliaryNodeInfo); // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("activeEventIds", m_activeEventIds);       // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("activeVariableIds", m_activeVariableIds); // offset: 44/64 size: 12/16 align: 4/8
    // class size: 56/80 align: 8/8
}

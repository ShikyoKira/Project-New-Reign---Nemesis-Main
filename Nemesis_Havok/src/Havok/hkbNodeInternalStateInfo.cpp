#include "Havok/hkbNodeInternalStateInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbNodeInternalStateInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbNodeInternalStateInfo::Class; });

nemesis::hkbNodeInternalStateInfo::hkbNodeInternalStateInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbNodeInternalStateInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbNodeInternalStateInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("syncInfo", m_syncInfo);           // offset: 8/16 size: 80/80 align: 4/4
    serializer.WriteValue("name", m_name);                    // offset: 88/96 size: 4/8 align: 4/8
    serializer.WriteObject("internalState", m_internalState); // offset: 92/104 size: 4/8 align: 4/8
    serializer.WriteValue("nodeId", m_nodeId);                // offset: 96/112 size: 2/2 align: 2/2
    serializer.WriteValue("hasActivateBeenCalled",
                          m_hasActivateBeenCalled); // offset: 98/114 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());    // offset: 99/115 size: 1/5
    // class size: 100/120 align: 4/8
}

void nemesis::hkbNodeInternalStateInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("syncInfo", m_syncInfo);            // offset: 8/16 size: 80/80 align: 4/4
    deserializer.ReadValue("name", m_name);                     // offset: 88/96 size: 4/8 align: 4/8
    deserializer.ReadObject("internalState", m_internalState);  // offset: 92/104 size: 4/8 align: 4/8
    deserializer.ReadValue("nodeId", m_nodeId);                 // offset: 96/112 size: 2/2 align: 2/2
    deserializer.ReadValue("hasActivateBeenCalled",
                           m_hasActivateBeenCalled); // offset: 98/114 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 99/115 size: 1/5
    // class size: 100/120 align: 4/8
}

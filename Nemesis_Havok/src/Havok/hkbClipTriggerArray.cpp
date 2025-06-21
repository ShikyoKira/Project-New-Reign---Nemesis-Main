#include "Havok/hkbClipTriggerArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClipTriggerArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClipTriggerArray::Class; });

nemesis::hkbClipTriggerArray::hkbClipTriggerArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbClipTriggerArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClipTriggerArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("triggers", m_triggers);       // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbClipTriggerArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("triggers", m_triggers);            // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

#include "Havok/hkbEventRangeDataArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventRangeDataArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventRangeDataArray::Class; });

nemesis::hkbEventRangeDataArray::hkbEventRangeDataArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventRangeDataArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventRangeDataArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("eventData", m_eventData);     // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbEventRangeDataArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("eventData", m_eventData);          // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

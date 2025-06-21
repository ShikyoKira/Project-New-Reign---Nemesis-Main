#include "Havok/hkpSerializedDisplayMarkerList.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedDisplayMarkerList,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedDisplayMarkerList::Class; });

nemesis::hkpSerializedDisplayMarkerList::hkpSerializedDisplayMarkerList() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpSerializedDisplayMarkerList::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedDisplayMarkerList::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("markers", m_markers);         // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkpSerializedDisplayMarkerList::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("markers", m_markers);              // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

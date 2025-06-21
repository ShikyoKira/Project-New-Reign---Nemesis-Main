#include "Havok/hkpSerializedDisplayMarker.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedDisplayMarker,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedDisplayMarker::Class; });

nemesis::hkpSerializedDisplayMarker::hkpSerializedDisplayMarker() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpSerializedDisplayMarker::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedDisplayMarker::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("transform", m_transform); // offset: 16/16 size: 64/64 align: 16/16
    // class size: 80/80 align: 16/16
}

void nemesis::hkpSerializedDisplayMarker::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("transform", m_transform); // offset: 16/16 size: 64/64 align: 16/16
    // class size: 80/80 align: 16/16
}

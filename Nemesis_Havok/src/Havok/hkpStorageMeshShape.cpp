#include "Havok/hkpStorageMeshShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStorageMeshShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStorageMeshShape::Class; });

nemesis::hkpStorageMeshShape::hkpStorageMeshShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpMeshShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpStorageMeshShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStorageMeshShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMeshShape::SerializeTo(serializer); // offset: 0/0 size: 96/128 align: 16/16
    serializer.WriteObject("storage", m_storage);   // offset: 96/128 size: 12/16 align: 4/8
    serializer.Pad(16);                             // offset: 108/144 size: 4/0
    // class size: 112/144 align: 16/16
}

void nemesis::hkpStorageMeshShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMeshShape::DeserializeFrom(deserializer); // offset: 0/0 size: 96/128 align: 16/16
    deserializer.ReadObject("storage", m_storage);        // offset: 96/128 size: 12/16 align: 4/8
    deserializer.Pad(16);                                 // offset: 108/144 size: 4/0
    // class size: 112/144 align: 16/16
}

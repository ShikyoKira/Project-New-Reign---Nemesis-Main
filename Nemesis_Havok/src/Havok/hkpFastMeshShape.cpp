#include "Havok/hkpFastMeshShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpFastMeshShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpFastMeshShape::Class; });

nemesis::hkpFastMeshShape::hkpFastMeshShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpMeshShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpFastMeshShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpFastMeshShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMeshShape::SerializeTo(serializer); // offset: 0/0 size: 96/128 align: 16/16
    // class size: 96/128 align: 16/16
}

void nemesis::hkpFastMeshShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMeshShape::DeserializeFrom(deserializer); // offset: 0/0 size: 96/128 align: 16/16
    // class size: 96/128 align: 16/16
}

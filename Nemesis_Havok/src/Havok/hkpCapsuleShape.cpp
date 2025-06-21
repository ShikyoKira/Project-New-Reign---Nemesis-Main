#include "Havok/hkpCapsuleShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCapsuleShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCapsuleShape::Class; });

nemesis::hkpCapsuleShape::hkpCapsuleShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpCapsuleShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCapsuleShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer); // offset: 0/0 size: 20/40 align: 4/8
    serializer.Pad(16);                               // offset: 20/40 size: 12/8

    serializer.WriteValue("vertexA", m_vertexA); // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("vertexB", m_vertexB); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

void nemesis::hkpCapsuleShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.Pad(16);                                   // offset: 20/40 size: 12/8

    deserializer.ReadValue("vertexA", m_vertexA); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("vertexB", m_vertexB); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

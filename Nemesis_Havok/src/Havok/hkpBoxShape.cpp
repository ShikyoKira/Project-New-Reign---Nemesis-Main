#include "Havok/hkpBoxShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBoxShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBoxShape::Class; });

nemesis::hkpBoxShape::hkpBoxShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpBoxShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBoxShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer); // offset: 0/0 size: 20/40 align: 4/8
    serializer.Pad(16);                               // offset: 20/40 size: 12/8

    serializer.WriteValue("halfExtents", m_halfExtents); // offset: 32/48 size: 16/16 align: 16/16
    // class size: 48/64 align: 16/16
}

void nemesis::hkpBoxShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.Pad(16);                                   // offset: 20/40 size: 12/8

    deserializer.ReadValue("halfExtents", m_halfExtents); // offset: 32/48 size: 16/16 align: 16/16
    // class size: 48/64 align: 16/16
}

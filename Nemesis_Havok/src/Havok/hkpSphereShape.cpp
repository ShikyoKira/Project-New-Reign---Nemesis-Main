#include "Havok/hkpSphereShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSphereShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSphereShape::Class; });

nemesis::hkpSphereShape::hkpSphereShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpSphereShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSphereShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer);        // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteSerializeIgnoredValue("pad16", m_pad16); // offset: 20/40 size: 12/12 align: 4/4
    serializer.Pad(serializer.GetPointerSize());             // offset: 32/52 size: 0/4
    // class size: 32/56 align: 4/8
}

void nemesis::hkpSphereShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadValue("pad16", m_pad16);               // offset: 20/40 size: 12/12 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());        // offset: 32/52 size: 0/4
    // class size: 32/56 align: 4/8
}

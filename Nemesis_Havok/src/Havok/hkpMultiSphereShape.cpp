#include "Havok/hkpMultiSphereShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMultiSphereShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMultiSphereShape::Class; });

nemesis::hkpMultiSphereShape::hkpMultiSphereShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpSphereRepShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpMultiSphereShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMultiSphereShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpSphereRepShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteValue("numSpheres", m_numSpheres);   // offset: 16/32 size: 4/4 align: 4/4
    serializer.Skip(12);                                 // offset: 20/36 size: 12/12

    serializer.WriteValue("spheres", m_spheres); // offset: 32/48 size: 128/128 align: 16/16
    // class size: 160/176 align: 16/16
}

void nemesis::hkpMultiSphereShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpSphereRepShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadValue("numSpheres", m_numSpheres);        // offset: 16/32 size: 4/4 align: 4/4
    deserializer.Skip(12);                                     // offset: 20/36 size: 12/12

    deserializer.ReadValue("spheres", m_spheres); // offset: 32/48 size: 128/128 align: 16/16
    // class size: 160/176 align: 16/16
}

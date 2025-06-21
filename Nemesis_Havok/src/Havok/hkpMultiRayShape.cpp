#include "Havok/hkpMultiRayShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMultiRayShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMultiRayShape::Class; });

nemesis::hkpMultiRayShape::hkpMultiRayShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpMultiRayShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMultiRayShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteObject("rays", m_rays);     // offset: 16/32 size: 12/16 align: 4/8
    serializer.WriteValue("rayPenetrationDistance",
                          m_rayPenetrationDistance); // offset: 28/48 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());     // offset: 32/52 size: 0/4
    // class size: 32/56 align: 4/8
}

void nemesis::hkpMultiRayShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadObject("rays", m_rays);          // offset: 16/32 size: 12/16 align: 4/8
    deserializer.ReadValue("rayPenetrationDistance",
                           m_rayPenetrationDistance); // offset: 28/48 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 32/52 size: 0/4
    // class size: 32/56 align: 4/8
}

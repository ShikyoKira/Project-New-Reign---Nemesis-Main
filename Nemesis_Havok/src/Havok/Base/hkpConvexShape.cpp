#include "Havok/Base/hkpConvexShape.h"

nemesis::hkpConvexShape::hkpConvexShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpSphereRepShape(cls)
{
}

void nemesis::hkpConvexShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpSphereRepShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteValue("radius", m_radius);           // offset: 16/32 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());         // offset: 20/36 size: 0/4
    // class size: 20/40 align: 4/8
}

void nemesis::hkpConvexShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpSphereRepShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadValue("radius", m_radius);                // offset: 16/32 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());           // offset: 20/36 size: 0/4
    // class size: 20/40 align: 4/8
}

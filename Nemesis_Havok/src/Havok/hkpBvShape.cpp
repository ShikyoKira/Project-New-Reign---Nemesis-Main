#include "Havok/hkpBvShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBvShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBvShape::Class; });

nemesis::hkpBvShape::hkpBvShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpBvShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBvShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteObject("boundingVolumeShape",
                           m_boundingVolumeShape);      // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteObject("childShape", m_childShape); // offset: 20/40 size: 12/16 align: 4/8
    // class size: 32/56 align: 4/8
}

void nemesis::hkpBvShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadObject("boundingVolumeShape",
                            m_boundingVolumeShape);      // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadObject("childShape", m_childShape); // offset: 20/40 size: 12/16 align: 4/8
    // class size: 32/56 align: 4/8
}

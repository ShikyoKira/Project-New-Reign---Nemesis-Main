#include "Havok/hkpPlaneShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPlaneShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPlaneShape::Class; });

nemesis::hkpPlaneShape::hkpPlaneShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpHeightFieldShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpPlaneShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPlaneShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpHeightFieldShape::SerializeTo(serializer);       // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteValue("plane", m_plane);                     // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteValue("aabbCenter", m_aabbCenter);           // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

void nemesis::hkpPlaneShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpHeightFieldShape::DeserializeFrom(deserializer);  // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadValue("plane", m_plane);                     // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadValue("aabbCenter", m_aabbCenter);           // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("aabbHalfExtents", m_aabbHalfExtents); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

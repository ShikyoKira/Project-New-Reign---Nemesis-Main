#include "Havok/hkpCylinderShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCylinderShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCylinderShape::Class; });

nemesis::hkpCylinderShape::hkpCylinderShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpCylinderShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCylinderShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer); // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteValue("cylRadius", m_cylRadius);  // offset: 20/40 size: 4/4 align: 4/4
    serializer.WriteValue(
        "cylBaseRadiusFactorForHeightFieldCollisions",
        m_cylBaseRadiusFactorForHeightFieldCollisions); // offset: 24/44 size: 4/4 align: 4/4
    serializer.Pad(16);                                 // offset: 28/48 size: 4/0

    serializer.WriteValue("vertexA", m_vertexA);               // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("vertexB", m_vertexB);               // offset: 48/64 size: 16/16 align: 16/16
    serializer.WriteValue("perpendicular1", m_perpendicular1); // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteValue("perpendicular2", m_perpendicular2); // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

void nemesis::hkpCylinderShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadValue("cylRadius", m_cylRadius);       // offset: 20/40 size: 4/4 align: 4/4
    deserializer.ReadValue(
        "cylBaseRadiusFactorForHeightFieldCollisions",
        m_cylBaseRadiusFactorForHeightFieldCollisions); // offset: 24/44 size: 4/4 align: 4/4
    deserializer.Pad(16);                               // offset: 28/48 size: 4/0

    deserializer.ReadValue("vertexA", m_vertexA);               // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("vertexB", m_vertexB);               // offset: 48/64 size: 16/16 align: 16/16
    deserializer.ReadValue("perpendicular1", m_perpendicular1); // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadValue("perpendicular2", m_perpendicular2); // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

#include "Havok/hkpTriangleShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTriangleShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTriangleShape::Class; });

nemesis::hkpTriangleShape::hkpTriangleShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpTriangleShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTriangleShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexShape::SerializeTo(serializer);    // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteValue("weldingInfo", m_weldingInfo); // offset: 20/40 size: 2/2 align: 2/2
    serializer.WriteValue("weldingType", m_weldingType); // offset: 22/42 size: 1/1 align: 1/1
    serializer.WriteValue("isExtruded", m_isExtruded);   // offset: 23/43 size: 1/1 align: 1/1
    serializer.Pad(16);                                  // offset: 24/44 size: 8/4

    serializer.WriteValue("vertexA", m_vertexA);     // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("vertexB", m_vertexB);     // offset: 48/64 size: 16/16 align: 16/16
    serializer.WriteValue("vertexC", m_vertexC);     // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteValue("extrusion", m_extrusion); // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

void nemesis::hkpTriangleShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadValue("weldingInfo", m_weldingInfo);   // offset: 20/40 size: 2/2 align: 2/2
    deserializer.ReadValue("weldingType", m_weldingType);   // offset: 22/42 size: 1/1 align: 1/1
    deserializer.ReadValue("isExtruded", m_isExtruded);     // offset: 23/43 size: 1/1 align: 1/1
    deserializer.Pad(16);                                   // offset: 24/44 size: 8/4

    deserializer.ReadValue("vertexA", m_vertexA);     // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("vertexB", m_vertexB);     // offset: 48/64 size: 16/16 align: 16/16
    deserializer.ReadValue("vertexC", m_vertexC);     // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadValue("extrusion", m_extrusion); // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

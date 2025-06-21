#include "Havok/hkpConvexVerticesShapeFourVectors.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexVerticesShapeFourVectors,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexVerticesShapeFourVectors::Class; });

nemesis::hkpConvexVerticesShapeFourVectors::hkpConvexVerticesShapeFourVectors() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpConvexVerticesShapeFourVectors::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexVerticesShapeFourVectors::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("x", m_x); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("y", m_y); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("z", m_z); // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}

void nemesis::hkpConvexVerticesShapeFourVectors::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("x", m_x); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("y", m_y); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("z", m_z); // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}

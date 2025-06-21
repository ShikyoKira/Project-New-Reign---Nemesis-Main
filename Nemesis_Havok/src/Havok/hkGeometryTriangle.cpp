#include "Havok/hkGeometryTriangle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkGeometryTriangle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkGeometryTriangle::Class; });

nemesis::hkGeometryTriangle::hkGeometryTriangle() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkGeometryTriangle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkGeometryTriangle::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("a", m_a);               // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("b", m_b);               // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("c", m_c);               // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("material", m_material); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkGeometryTriangle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("a", m_a);               // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("b", m_b);               // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("c", m_c);               // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("material", m_material); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

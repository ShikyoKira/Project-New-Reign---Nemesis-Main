#include "Havok/hkAabb.h"

REGISTER_HAVOK_POINTER_SOURCE(hkAabb,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkAabb::Class; });

nemesis::hkAabb::hkAabb() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkAabb::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkAabb::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("min", m_min); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("max", m_max); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkAabb::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("min", m_min); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("max", m_max); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

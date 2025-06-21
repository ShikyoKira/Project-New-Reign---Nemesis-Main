#include "Havok/hkSphere.h"

REGISTER_HAVOK_POINTER_SOURCE(hkSphere,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSphere::Class; });

nemesis::hkSphere::hkSphere() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkSphere::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkSphere::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("pos", m_pos); // offset: 0/0 size: 16/16 align: 16/16
    // class size: 16/16 align: 16/16
}

void nemesis::hkSphere::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("pos", m_pos); // offset: 0/0 size: 16/16 align: 16/16
    // class size: 16/16 align: 16/16
}

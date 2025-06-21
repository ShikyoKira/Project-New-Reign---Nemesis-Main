#include "Havok/hkbRoleAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRoleAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRoleAttribute::Class; });

nemesis::hkbRoleAttribute::hkbRoleAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbRoleAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRoleAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("role", m_role);   // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("flags", m_flags); // offset: 2/2 size: 2/2 align: 2/2
    // class size: 4/4 align: 2/2
}

void nemesis::hkbRoleAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("role", m_role);   // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("flags", m_flags); // offset: 2/2 size: 2/2 align: 2/2
    // class size: 4/4 align: 2/2
}

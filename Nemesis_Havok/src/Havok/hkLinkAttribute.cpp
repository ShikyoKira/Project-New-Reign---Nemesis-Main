#include "Havok/hkLinkAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkLinkAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkLinkAttribute::Class; });

nemesis::hkLinkAttribute::hkLinkAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkLinkAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkLinkAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkLinkAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

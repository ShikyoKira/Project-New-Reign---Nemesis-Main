#include "Havok/hkRootLevelContainerNamedVariant.h"

REGISTER_HAVOK_POINTER_SOURCE(hkRootLevelContainerNamedVariant,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkRootLevelContainerNamedVariant::Class; });

nemesis::hkRootLevelContainerNamedVariant::hkRootLevelContainerNamedVariant() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkRootLevelContainerNamedVariant::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkRootLevelContainerNamedVariant::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);           // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("className", m_className); // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteObject("variant", m_variant);    // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkRootLevelContainerNamedVariant::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);           // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("className", m_className); // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadObject("variant", m_variant);    // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

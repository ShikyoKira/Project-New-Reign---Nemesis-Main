#include "Havok/hkxAttributeGroup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxAttributeGroup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxAttributeGroup::Class; });

nemesis::hkxAttributeGroup::hkxAttributeGroup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxAttributeGroup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxAttributeGroup::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);              // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("attributes", m_attributes); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}

void nemesis::hkxAttributeGroup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);              // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("attributes", m_attributes); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}

#include "Havok/hkCustomAttributesAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkCustomAttributesAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkCustomAttributesAttribute::Class; });

nemesis::hkCustomAttributesAttribute::hkCustomAttributesAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkCustomAttributesAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkCustomAttributesAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);    // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("value", m_value); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkCustomAttributesAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);    // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("value", m_value); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

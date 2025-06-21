#include "Havok/hkCustomAttributes.h"

REGISTER_HAVOK_POINTER_SOURCE(hkCustomAttributes,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkCustomAttributes::Class; });

nemesis::hkCustomAttributes::hkCustomAttributes() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkCustomAttributes::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkCustomAttributes::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("attributes", m_attributes); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkCustomAttributes::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("attributes", m_attributes); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

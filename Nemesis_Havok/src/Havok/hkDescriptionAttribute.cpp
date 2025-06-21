#include "Havok/hkDescriptionAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkDescriptionAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkDescriptionAttribute::Class; });

nemesis::hkDescriptionAttribute::hkDescriptionAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkDescriptionAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkDescriptionAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("string", m_string); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkDescriptionAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("string", m_string); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

#include "Havok/hkClassEnum.h"

REGISTER_HAVOK_POINTER_SOURCE(hkClassEnum,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkClassEnum::Class; });

nemesis::hkClassEnum::hkClassEnum() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkClassEnum::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkClassEnum::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);                              // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("items", m_items);                           // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("attributes", m_attributes); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("flags", m_flags);                            // offset: 12/24 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                        // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

void nemesis::hkClassEnum::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);              // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("items", m_items);           // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadObject("attributes", m_attributes); // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("flags", m_flags);            // offset: 12/24 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

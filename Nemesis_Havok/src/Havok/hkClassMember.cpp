#include "Havok/hkClassMember.h"

REGISTER_HAVOK_POINTER_SOURCE(hkClassMember,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkClassMember::Class; });

nemesis::hkClassMember::hkClassMember() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkClassMember::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkClassMember::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);                              // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("class", m_class);                           // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteObject("enum", m_enum);                             // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("type", m_type);                              // offset: 12/24 size: 1/1 align: 1/1
    serializer.WriteValue("subtype", m_subtype);                        // offset: 13/25 size: 1/1 align: 1/1
    serializer.WriteValue("cArraySize", m_cArraySize);                  // offset: 14/26 size: 2/2 align: 2/2
    serializer.WriteValue("flags", m_flags);                            // offset: 16/28 size: 2/2 align: 2/2
    serializer.WriteValue("offset", m_offset);                          // offset: 18/30 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredObject("attributes", m_attributes); // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkClassMember::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);              // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("class", m_class);           // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadObject("enum", m_enum);             // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);              // offset: 12/24 size: 1/1 align: 1/1
    deserializer.ReadValue("subtype", m_subtype);        // offset: 13/25 size: 1/1 align: 1/1
    deserializer.ReadValue("cArraySize", m_cArraySize);  // offset: 14/26 size: 2/2 align: 2/2
    deserializer.ReadValue("flags", m_flags);            // offset: 16/28 size: 2/2 align: 2/2
    deserializer.ReadValue("offset", m_offset);          // offset: 18/30 size: 2/2 align: 2/2
    deserializer.ReadObject("attributes", m_attributes); // offset: 20/32 size: 4/8 align: 4/8
    // class size: 24/40 align: 4/8
}

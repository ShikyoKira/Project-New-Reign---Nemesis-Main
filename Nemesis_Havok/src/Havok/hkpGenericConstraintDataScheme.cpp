#include "Havok/hkpGenericConstraintDataScheme.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpGenericConstraintDataScheme,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpGenericConstraintDataScheme::Class; });

nemesis::hkpGenericConstraintDataScheme::hkpGenericConstraintDataScheme() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpGenericConstraintDataScheme::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpGenericConstraintDataScheme::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("info", m_info);           // offset: 0/0 size: 16/16 align: 4/4
    serializer.WriteObject("data", m_data);                           // offset: 16/16 size: 12/16 align: 4/8
    serializer.WriteObject("commands", m_commands);                   // offset: 28/32 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("modifiers", m_modifiers); // offset: 40/48 size: 12/16 align: 4/8
    serializer.WriteObject("motors", m_motors);                       // offset: 52/64 size: 12/16 align: 4/8
    // class size: 64/80 align: 4/8
}

void nemesis::hkpGenericConstraintDataScheme::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("info", m_info);           // offset: 0/0 size: 16/16 align: 4/4
    deserializer.ReadObject("data", m_data);           // offset: 16/16 size: 12/16 align: 4/8
    deserializer.ReadObject("commands", m_commands);   // offset: 28/32 size: 12/16 align: 4/8
    deserializer.ReadObject("modifiers", m_modifiers); // offset: 40/48 size: 12/16 align: 4/8
    deserializer.ReadObject("motors", m_motors);       // offset: 52/64 size: 12/16 align: 4/8
    // class size: 64/80 align: 4/8
}

#include "Havok/hkxEnvironmentVariable.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxEnvironmentVariable,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxEnvironmentVariable::Class; });

nemesis::hkxEnvironmentVariable::hkxEnvironmentVariable() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxEnvironmentVariable::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxEnvironmentVariable::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);   // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("value", m_value); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkxEnvironmentVariable::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);   // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("value", m_value); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

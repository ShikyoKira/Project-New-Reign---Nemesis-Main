#include "Havok/hkbVariableValue.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableValue,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableValue::Class; });

nemesis::hkbVariableValue::hkbVariableValue() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbVariableValue::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableValue::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("value", m_value); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

void nemesis::hkbVariableValue::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("value", m_value); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

#include "Havok/hkSimplePropertyValue.h"

nemesis::hkPropertyValueCommon::hkPropertyValueCommon() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

void nemesis::hkPropertyValueCommon::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("data", m_data); // offset: 0/0 size: 8/8 align: 8/8
    // class size: 8/8 align: 8/8
}

void nemesis::hkPropertyValueCommon::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("data", m_data); // offset: 0/0 size: 8/8 align: 8/8
    // class size: 8/8 align: 8/8
}

REGISTER_HAVOK_POINTER_SOURCE(hkpPropertyValue,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPropertyValue::Class; });

const nemesis::hkClass* nemesis::hkpPropertyValue::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

REGISTER_HAVOK_POINTER_SOURCE(hkSimplePropertyValue,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSimplePropertyValue::Class; });

const nemesis::hkClass* nemesis::hkSimplePropertyValue::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

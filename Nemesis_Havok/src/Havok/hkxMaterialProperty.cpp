#include "Havok/hkxMaterialProperty.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterialProperty,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterialProperty::Class; });

nemesis::hkxMaterialProperty::hkxMaterialProperty() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

void nemesis::hkxMaterialProperty::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("key", m_key);     // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkxMaterialProperty::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("key", m_key);     // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("value", m_value); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

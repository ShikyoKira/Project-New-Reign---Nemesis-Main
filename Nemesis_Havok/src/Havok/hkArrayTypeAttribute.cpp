#include "Havok/hkArrayTypeAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkArrayTypeAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkArrayTypeAttribute::Class; });

nemesis::hkArrayTypeAttribute::hkArrayTypeAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkArrayTypeAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkArrayTypeAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkArrayTypeAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

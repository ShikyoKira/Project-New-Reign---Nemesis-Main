#include "Havok/hkDataObjectTypeAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkDataObjectTypeAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkDataObjectTypeAttribute::Class; });

nemesis::hkDataObjectTypeAttribute::hkDataObjectTypeAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkDataObjectTypeAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkDataObjectTypeAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("typeName", m_typeName); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkDataObjectTypeAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("typeName", m_typeName); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

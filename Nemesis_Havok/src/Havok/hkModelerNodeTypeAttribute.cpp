#include "Havok/hkModelerNodeTypeAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkModelerNodeTypeAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkModelerNodeTypeAttribute::Class; });

nemesis::hkModelerNodeTypeAttribute::hkModelerNodeTypeAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkModelerNodeTypeAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkModelerNodeTypeAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkModelerNodeTypeAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("type", m_type); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

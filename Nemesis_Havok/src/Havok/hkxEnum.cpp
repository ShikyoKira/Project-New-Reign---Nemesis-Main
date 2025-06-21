#include "Havok/hkxEnum.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxEnum,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxEnum::Class; });

nemesis::hkxEnum::hkxEnum() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxEnum::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxEnum::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("items", m_items);             // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxEnum::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("items", m_items);                  // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

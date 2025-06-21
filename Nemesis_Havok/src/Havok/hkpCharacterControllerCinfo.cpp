#include "Havok/hkpCharacterControllerCinfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCharacterControllerCinfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCharacterControllerCinfo::Class; });

nemesis::hkpCharacterControllerCinfo::hkpCharacterControllerCinfo() noexcept
    : nemesis::hkpCharacterControllerCinfo(Class)
{
}

nemesis::hkpCharacterControllerCinfo::hkpCharacterControllerCinfo(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkpCharacterControllerCinfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCharacterControllerCinfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkpCharacterControllerCinfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

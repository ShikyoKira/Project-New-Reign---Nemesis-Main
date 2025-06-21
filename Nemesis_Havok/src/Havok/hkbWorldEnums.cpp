#include "Havok/hkbWorldEnums.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbWorldEnums,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbWorldEnums::Class; });

nemesis::hkbWorldEnums::hkbWorldEnums() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbWorldEnums::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbWorldEnums::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkbWorldEnums::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

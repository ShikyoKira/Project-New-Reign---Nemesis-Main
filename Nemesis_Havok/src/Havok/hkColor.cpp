#include "Havok/hkColor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkColor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkColor::Class; });

nemesis::hkColor::hkColor() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkColor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkColor::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0);
}

void nemesis::hkColor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0);
}

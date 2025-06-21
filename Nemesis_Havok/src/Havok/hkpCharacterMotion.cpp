#include "Havok/hkpCharacterMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCharacterMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCharacterMotion::Class; });

nemesis::hkpCharacterMotion::hkpCharacterMotion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpMotion(Class)
{
}

const nemesis::hkClass* nemesis::hkpCharacterMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCharacterMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMotion::SerializeTo(serializer);        // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpCharacterMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMotion::DeserializeFrom(deserializer);        // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

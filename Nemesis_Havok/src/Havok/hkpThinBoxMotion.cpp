#include "Havok/hkpThinBoxMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpThinBoxMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpThinBoxMotion::Class; });

nemesis::hkpThinBoxMotion::hkpThinBoxMotion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBoxMotion(Class)
{
}

const nemesis::hkClass* nemesis::hkpThinBoxMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpThinBoxMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBoxMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpThinBoxMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBoxMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

#include "Havok/hkpBoxMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBoxMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBoxMotion::Class; });

nemesis::hkpBoxMotion::hkpBoxMotion() noexcept
    : nemesis::hkpBoxMotion(Class)
{
}

nemesis::hkpBoxMotion::hkpBoxMotion(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpMotion(cls)
{
}

const nemesis::hkClass* nemesis::hkpBoxMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBoxMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpBoxMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

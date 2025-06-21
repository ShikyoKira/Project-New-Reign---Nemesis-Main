#include "Havok/hkpMountedBallGun.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMountedBallGun,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMountedBallGun::Class; });

nemesis::hkpMountedBallGun::hkpMountedBallGun() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBallGun(Class)
{
}

const nemesis::hkClass* nemesis::hkpMountedBallGun::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMountedBallGun::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBallGun::SerializeTo(serializer);  // offset: 0/0 size: 96/112 align: 16/16
    serializer.WriteValue("position", m_position); // offset: 96/112 size: 16/16 align: 16/16
    // class size: 112/128 align: 16/16
}

void nemesis::hkpMountedBallGun::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBallGun::DeserializeFrom(deserializer); // offset: 0/0 size: 96/112 align: 16/16
    deserializer.ReadValue("position", m_position);     // offset: 96/112 size: 16/16 align: 16/16
    // class size: 112/128 align: 16/16
}

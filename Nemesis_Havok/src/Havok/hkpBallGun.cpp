#include "Havok/hkpBallGun.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBallGun,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBallGun::Class; });

nemesis::hkpBallGun::hkpBallGun() noexcept
    : nemesis::hkpBallGun(Class)
{
}

nemesis::hkpBallGun::hkpBallGun(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpFirstPersonGun(cls)
{
}

const nemesis::hkClass* nemesis::hkpBallGun::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBallGun::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpFirstPersonGun::SerializeTo(serializer);             // offset: 0/0 size: 32/56 align: 4/8
    serializer.WriteValue("bulletRadius", m_bulletRadius);           // offset: 32/56 size: 4/4 align: 4/4
    serializer.WriteValue("bulletVelocity", m_bulletVelocity);       // offset: 36/60 size: 4/4 align: 4/4
    serializer.WriteValue("bulletMass", m_bulletMass);               // offset: 40/64 size: 4/4 align: 4/4
    serializer.WriteValue("damageMultiplier", m_damageMultiplier);   // offset: 44/68 size: 4/4 align: 4/4
    serializer.WriteValue("maxBulletsInWorld", m_maxBulletsInWorld); // offset: 48/72 size: 4/4 align: 4/4
    serializer.Pad(16);                                              // offset: 52/76 size: 12/4

    serializer.WriteValue("bulletOffsetFromCenter",
                          m_bulletOffsetFromCenter); // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredObject("addedBodies",
                                           m_addedBodies); // offset: 80/96 size: 4/8 align: 4/8
    serializer.Pad(16);                                    // offset: 84/104 size: 12/8
    // class size: 96/112 align: 16/16
}

void nemesis::hkpBallGun::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpFirstPersonGun::DeserializeFrom(deserializer);        // offset: 0/0 size: 32/56 align: 4/8
    deserializer.ReadValue("bulletRadius", m_bulletRadius);           // offset: 32/56 size: 4/4 align: 4/4
    deserializer.ReadValue("bulletVelocity", m_bulletVelocity);       // offset: 36/60 size: 4/4 align: 4/4
    deserializer.ReadValue("bulletMass", m_bulletMass);               // offset: 40/64 size: 4/4 align: 4/4
    deserializer.ReadValue("damageMultiplier", m_damageMultiplier);   // offset: 44/68 size: 4/4 align: 4/4
    deserializer.ReadValue("maxBulletsInWorld", m_maxBulletsInWorld); // offset: 48/72 size: 4/4 align: 4/4
    deserializer.Pad(16);                                             // offset: 52/76 size: 12/4

    deserializer.ReadValue("bulletOffsetFromCenter",
                           m_bulletOffsetFromCenter);      // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadObject("addedBodies", m_addedBodies); // offset: 80/96 size: 4/8 align: 4/8
    deserializer.Pad(16);                                  // offset: 84/104 size: 12/8
    // class size: 96/112 align: 16/16
}

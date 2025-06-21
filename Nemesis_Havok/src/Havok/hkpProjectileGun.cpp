#include "Havok/hkpProjectileGun.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpProjectileGun,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpProjectileGun::Class; });

nemesis::hkpProjectileGun::hkpProjectileGun() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpFirstPersonGun(Class)
{
}

const nemesis::hkClass* nemesis::hkpProjectileGun::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpProjectileGun::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpFirstPersonGun::SerializeTo(serializer);       // offset: 0/0 size: 32/56 align: 4/8
    serializer.WriteValue("maxProjectiles", m_maxProjectiles); // offset: 32/56 size: 4/4 align: 4/4
    serializer.WriteValue("reloadTime", m_reloadTime);         // offset: 36/60 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("reload", m_reload); // offset: 40/64 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());               // offset: 44/68 size: 0/4

    serializer.WriteSerializeIgnoredObject("projectiles",
                                           m_projectiles);    // offset: 44/72 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("world", m_world); // offset: 56/88 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("destructionWorld",
                                           m_destructionWorld); // offset: 60/96 size: 4/8 align: 4/8
    // class size: 64/104 align: 4/8
}

void nemesis::hkpProjectileGun::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpFirstPersonGun::DeserializeFrom(deserializer);  // offset: 0/0 size: 32/56 align: 4/8
    deserializer.ReadValue("maxProjectiles", m_maxProjectiles); // offset: 32/56 size: 4/4 align: 4/4
    deserializer.ReadValue("reloadTime", m_reloadTime);         // offset: 36/60 size: 4/4 align: 4/4
    deserializer.ReadValue("reload", m_reload);                 // offset: 40/64 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 44/68 size: 0/4

    deserializer.ReadObject("projectiles", m_projectiles);           // offset: 44/72 size: 12/16 align: 4/8
    deserializer.ReadObject("world", m_world);                       // offset: 56/88 size: 4/8 align: 4/8
    deserializer.ReadObject("destructionWorld", m_destructionWorld); // offset: 60/96 size: 4/8 align: 4/8
    // class size: 64/104 align: 4/8
}

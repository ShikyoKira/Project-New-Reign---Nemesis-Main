#include "Havok/hkpLinkedCollidable.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinkedCollidable,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinkedCollidable::Class; });

nemesis::hkpLinkedCollidable::hkpLinkedCollidable() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollidable(Class)
{
}

const nemesis::hkClass* nemesis::hkpLinkedCollidable::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinkedCollidable::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollidable::SerializeTo(serializer); // offset: 0/0 size: 80/112 align: 4/8
    serializer.WriteSerializeIgnoredObject("collisionEntries",
                                           m_collisionEntries); // offset: 80/112 size: 12/16 align: 4/8
    // class size: 92/128 align: 4/8
}

void nemesis::hkpLinkedCollidable::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollidable::DeserializeFrom(deserializer);           // offset: 0/0 size: 80/112 align: 4/8
    deserializer.ReadObject("collisionEntries", m_collisionEntries); // offset: 80/112 size: 12/16 align: 4/8
    // class size: 92/128 align: 4/8
}

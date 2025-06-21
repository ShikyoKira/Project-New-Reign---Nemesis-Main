#include "Havok/hkpCachingShapePhantom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCachingShapePhantom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCachingShapePhantom::Class; });

nemesis::hkpCachingShapePhantom::hkpCachingShapePhantom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShapePhantom(Class)
{
}

const nemesis::hkClass* nemesis::hkpCachingShapePhantom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCachingShapePhantom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapePhantom::SerializeTo(serializer); // offset: 0/0 size: 352/416 align: 16/16
    serializer.WriteSerializeIgnoredObject("collisionDetails",
                                           m_collisionDetails); // offset: 352/416 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("orderDirty", m_orderDirty); // offset: 364/432 size: 1/1 align: 1/1
    serializer.Pad(16);                                                // offset: 365/433 size: 3/15
    // class size: 368/448 align: 16/16
}

void nemesis::hkpCachingShapePhantom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapePhantom::DeserializeFrom(deserializer);         // offset: 0/0 size: 352/416 align: 16/16
    deserializer.ReadObject("collisionDetails", m_collisionDetails); // offset: 352/416 size: 12/16 align: 4/8
    deserializer.ReadValue("orderDirty", m_orderDirty);              // offset: 364/432 size: 1/1 align: 1/1
    deserializer.Pad(16);                                            // offset: 365/433 size: 3/15
    // class size: 368/448 align: 16/16
}

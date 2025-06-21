#include "Havok/hkpAabbPhantom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAabbPhantom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAabbPhantom::Class; });

nemesis::hkpAabbPhantom::hkpAabbPhantom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpPhantom(Class)
{
}

const nemesis::hkClass* nemesis::hkpAabbPhantom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAabbPhantom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpPhantom::SerializeTo(serializer); // offset: 0/0 size: 164/240 align: 4/8
    serializer.Pad(16);                           // offset: 164/240 size: 12/0

    serializer.WriteObject("aabb", m_aabb); // offset: 176/240 size: 32/32 align: 16/16
    serializer.WriteSerializeIgnoredObject(
        "overlappingCollidables", m_overlappingCollidables); // offset: 208/272 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("orderDirty", m_orderDirty); // offset: 220/288 size: 1/1 align: 1/1
    serializer.Pad(16);                                                // offset: 221/289 size: 3/15
    // class size: 224/304 align: 16/16
}

void nemesis::hkpAabbPhantom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpPhantom::DeserializeFrom(deserializer); // offset: 0/0 size: 164/240 align: 4/8
    deserializer.Pad(16);                               // offset: 164/240 size: 12/0

    deserializer.ReadObject("aabb", m_aabb); // offset: 176/240 size: 32/32 align: 16/16
    deserializer.ReadObject("overlappingCollidables",
                            m_overlappingCollidables);  // offset: 208/272 size: 12/16 align: 4/8
    deserializer.ReadValue("orderDirty", m_orderDirty); // offset: 220/288 size: 1/1 align: 1/1
    deserializer.Pad(16);                               // offset: 221/289 size: 3/15
    // class size: 224/304 align: 16/16
}

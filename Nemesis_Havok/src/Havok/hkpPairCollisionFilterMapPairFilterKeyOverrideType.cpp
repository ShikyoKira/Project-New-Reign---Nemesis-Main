#include "Havok/hkpPairCollisionFilterMapPairFilterKeyOverrideType.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPairCollisionFilterMapPairFilterKeyOverrideType,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::Class; });

nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::hkpPairCollisionFilterMapPairFilterKeyOverrideType() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("elem", m_elem); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("numElems", m_numElems);          // offset: 4/8 size: 4/4 align: 4/4
    serializer.WriteValue("hashMod", m_hashMod);            // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}

void nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("elem", m_elem);        // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("numElems", m_numElems); // offset: 4/8 size: 4/4 align: 4/4
    deserializer.ReadValue("hashMod", m_hashMod);   // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}

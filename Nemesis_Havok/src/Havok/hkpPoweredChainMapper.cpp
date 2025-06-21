#include "Havok/hkpPoweredChainMapper.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPoweredChainMapper,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPoweredChainMapper::Class; });

nemesis::hkpPoweredChainMapper::hkpPoweredChainMapper() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpPoweredChainMapper::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPoweredChainMapper::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("links", m_links);             // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("targets", m_targets);         // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("chains", m_chains);           // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}

void nemesis::hkpPoweredChainMapper::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("links", m_links);                  // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("targets", m_targets);              // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("chains", m_chains);                // offset: 32/48 size: 12/16 align: 4/8
    // class size: 44/64 align: 4/8
}

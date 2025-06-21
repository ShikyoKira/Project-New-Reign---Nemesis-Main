#include "Havok/hkpAgentNnEntry.h"

nemesis::hkpAgentNnEntry::hkpAgentNnEntry(const nemesis::hkClass& cls) noexcept
    : nemesis::hkpAgentEntry(cls)
{
}

const nemesis::hkClass* nemesis::hkpAgentNnEntry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAgentNnEntry::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpAgentEntry::SerializeTo(serializer); // offset: 0/0 size: 4/4 align: 1/1
    serializer.WriteValue("agentIndexOnCollidable",
                          m_agentIndexOnCollidable);    // offset: 4/4 size: 4/4 align: 2/2
    serializer.WriteObject("contactMgr", m_contactMgr); // offset: 8/8 size: 4/8 align: 4/8
    serializer.WriteValue("collisionQualityIndex",
                          m_collisionQualityIndex);                      // offset: 12/16 size: 1/1 align: 1/1
    serializer.WriteValue("forceCollideOntoPpu", m_forceCollideOntoPpu); // offset: 13/17 size: 1/1 align: 1/1
    serializer.WriteValue("nnTrackType", m_nnTrackType);                 // offset: 14/18 size: 1/1 align: 1/1
    serializer.WriteValue("padding", m_padding);                         // offset: 15/19 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                         // offset: 16/20 size: 0/4

    serializer.WriteObject("collidable", m_collidable); // offset: 16/24 size: 8/16 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkpAgentNnEntry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpAgentEntry::DeserializeFrom(deserializer); // offset: 0/0 size: 4/4 align: 1/1
    deserializer.ReadValue("agentIndexOnCollidable",
                           m_agentIndexOnCollidable);    // offset: 4/4 size: 4/4 align: 2/2
    deserializer.ReadObject("contactMgr", m_contactMgr); // offset: 8/8 size: 4/8 align: 4/8
    deserializer.ReadValue("collisionQualityIndex",
                           m_collisionQualityIndex); // offset: 12/16 size: 1/1 align: 1/1
    deserializer.ReadValue("forceCollideOntoPpu",
                           m_forceCollideOntoPpu);        // offset: 13/17 size: 1/1 align: 1/1
    deserializer.ReadValue("nnTrackType", m_nnTrackType); // offset: 14/18 size: 1/1 align: 1/1
    deserializer.ReadValue("padding", m_padding);         // offset: 15/19 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 16/20 size: 0/4

    deserializer.ReadObject("collidable", m_collidable); // offset: 16/24 size: 8/16 align: 4/8
    // class size: 24/40 align: 4/8
}

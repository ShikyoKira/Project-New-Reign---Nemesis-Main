#include "Havok/hkpSerializedAgentNnEntry.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedAgentNnEntry,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedAgentNnEntry::Class; });

nemesis::hkpSerializedAgentNnEntry::hkpSerializedAgentNnEntry() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpSerializedAgentNnEntry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedAgentNnEntry::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);  // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("bodyA", m_bodyA);              // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("bodyB", m_bodyB);              // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("bodyAId", m_bodyAId);           // offset: 16/32 size: 8/8 align: 8/8
    serializer.WriteValue("bodyBId", m_bodyBId);           // offset: 24/40 size: 8/8 align: 8/8
    serializer.WriteValue("useEntityIds", m_useEntityIds); // offset: 32/48 size: 1/1 align: 1/1
    serializer.WriteValue("agentType", m_agentType);       // offset: 33/49 size: 1/1 align: 1/1
    serializer.Skip(14);                                   // offset: 34/50 size: 14/14

    serializer.WriteObject("atom", m_atom);                         // offset: 48/64 size: 48/48 align: 16/16
    serializer.WriteObject("propertiesStream", m_propertiesStream); // offset: 96/112 size: 12/16 align: 4/8
    serializer.WriteObject("contactPoints", m_contactPoints);       // offset: 108/128 size: 12/16 align: 4/8
    serializer.WriteObject("cpIdMgr", m_cpIdMgr);                   // offset: 120/144 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("nnEntryData", m_nnEntryData_1);
    }
    else
    {
        serializer.WriteValue("nnEntryData", m_nnEntryData_0); // offset: 132/160 size: 160/160 align: 1/1
    }

    serializer.WriteObject("trackInfo", m_trackInfo);                // offset: 292/320 size: 24/32 align: 4/8
    serializer.WriteValue("endianCheckBuffer", m_endianCheckBuffer); // offset: 316/352 size: 4/4 align: 1/1
    serializer.WriteValue("version", m_version);                     // offset: 320/356 size: 4/4 align: 4/4
    serializer.Pad(16);                                              // offset: 324/360 size: 12/8
    // class size: 336/368 align: 16/16
}

void nemesis::hkpSerializedAgentNnEntry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("bodyA", m_bodyA);                  // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("bodyB", m_bodyB);                  // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("bodyAId", m_bodyAId);               // offset: 16/32 size: 8/8 align: 8/8
    deserializer.ReadValue("bodyBId", m_bodyBId);               // offset: 24/40 size: 8/8 align: 8/8
    deserializer.ReadValue("useEntityIds", m_useEntityIds);     // offset: 32/48 size: 1/1 align: 1/1
    deserializer.ReadValue("agentType", m_agentType);           // offset: 33/49 size: 1/1 align: 1/1
    deserializer.Skip(14);                                      // offset: 34/50 size: 14/14

    deserializer.ReadObject("atom", m_atom);                         // offset: 48/64 size: 48/48 align: 16/16
    deserializer.ReadObject("propertiesStream", m_propertiesStream); // offset: 96/112 size: 12/16 align: 4/8
    deserializer.ReadObject("contactPoints", m_contactPoints);       // offset: 108/128 size: 12/16 align: 4/8
    deserializer.ReadObject("cpIdMgr", m_cpIdMgr);                   // offset: 120/144 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("nnEntryData", m_nnEntryData_1);

        for (size_t i = 0; i < m_nnEntryData_0.size(); i++)
        {
            m_nnEntryData_0[i] = m_nnEntryData_1[i];
        }
    }
    else
    {
        deserializer.ReadValue("nnEntryData", m_nnEntryData_0); // offset: 132/160 size: 160/160 align: 1/1

        for (size_t i = 0; i < m_nnEntryData_0.size(); i++)
        {
            m_nnEntryData_1[i] = m_nnEntryData_0[i];
        }
    }

    deserializer.ReadObject("trackInfo", m_trackInfo); // offset: 292/320 size: 24/32 align: 4/8
    deserializer.ReadValue("endianCheckBuffer", m_endianCheckBuffer); // offset: 316/352 size: 4/4 align: 1/1
    deserializer.ReadValue("version", m_version);                     // offset: 320/356 size: 4/4 align: 4/4
    deserializer.Pad(16);                                             // offset: 324/360 size: 12/8
    // class size: 336/368 align: 16/16
}

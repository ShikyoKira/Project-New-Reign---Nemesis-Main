#include "Havok/hkpAgentEntry.h"

nemesis::hkpAgentEntry::hkpAgentEntry(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpAgentEntry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAgentEntry::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("streamCommand", m_streamCommand);       // offset: 0/0 size: 1/1 align: 1/1
    serializer.WriteValue("agentType", m_agentType);               // offset: 1/1 size: 1/1 align: 1/1
    serializer.WriteValue("numContactPoints", m_numContactPoints); // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("size", m_size);                         // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 1/1
}

void nemesis::hkpAgentEntry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("streamCommand", m_streamCommand);       // offset: 0/0 size: 1/1 align: 1/1
    deserializer.ReadValue("agentType", m_agentType);               // offset: 1/1 size: 1/1 align: 1/1
    deserializer.ReadValue("numContactPoints", m_numContactPoints); // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("size", m_size);                         // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 1/1
}

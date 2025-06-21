#include "Havok/hkpSerializedSubTrack1nInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedSubTrack1nInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedSubTrack1nInfo::Class; });

nemesis::hkpSerializedSubTrack1nInfo::hkpSerializedSubTrack1nInfo() noexcept
    : nemesis::hkpSerializedTrack1nInfo(Class)
{
}

const nemesis::hkClass* nemesis::hkpSerializedSubTrack1nInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedSubTrack1nInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpSerializedTrack1nInfo::SerializeTo(serializer); // offset: 0/0 size: 24/32 align: 4/8
    serializer.WriteValue("sectorIndex", m_sectorIndex);        // offset: 24/32 size: 4/4 align: 4/4
    serializer.WriteValue("offsetInSector", m_offsetInSector);  // offset: 28/36 size: 4/4 align: 4/4
    // class size: 32/40 align: 4/8
}

void nemesis::hkpSerializedSubTrack1nInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpSerializedTrack1nInfo::DeserializeFrom(deserializer); // offset: 0/0 size: 24/32 align: 4/8
    deserializer.ReadValue("sectorIndex", m_sectorIndex);             // offset: 24/32 size: 4/4 align: 4/4
    deserializer.ReadValue("offsetInSector", m_offsetInSector);       // offset: 28/36 size: 4/4 align: 4/4
    // class size: 32/40 align: 4/8
}

#include "Havok/hkbGeneratorSyncInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorSyncInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorSyncInfo::Class; });

nemesis::hkbGeneratorSyncInfo::hkbGeneratorSyncInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbGeneratorSyncInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorSyncInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("syncPoints", m_syncPoints);
        serializer.WriteValue("duration", m_duration);
    }
    else
    {
        std::array<nemesis::hkbGeneratorSyncInfoSyncPoint, 8> syncPoints;

        for (size_t i = 0; i < syncPoints.size(); ++i)
        {
            syncPoints[i] = m_syncPoints[i];
        }

        serializer.WriteObject("syncPoints", syncPoints);        // offset: 0/0 size: 64/64 align: 4/4
        serializer.WriteValue("baseFrequency", m_baseFrequency); // offset: 64/64 size: 4/4 align: 4/4
    }

    serializer.WriteValue("localTime", m_localTime);         // offset: 68/68 size: 4/4 align: 4/4
    serializer.WriteValue("playbackSpeed", m_playbackSpeed); // offset: 72/72 size: 4/4 align: 4/4
    serializer.WriteValue("numSyncPoints", m_numSyncPoints); // offset: 76/76 size: 1/1 align: 1/1
    serializer.WriteValue("isCyclic", m_isCyclic);           // offset: 77/77 size: 1/1 align: 1/1
    serializer.WriteValue("isMirrored", m_isMirrored);       // offset: 78/78 size: 1/1 align: 1/1
    serializer.WriteValue("isAdditive", m_isAdditive);       // offset: 79/79 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("activeInterval", m_activeInterval);
    }

    // class size: 80/80 align: 4/4
}

void nemesis::hkbGeneratorSyncInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("syncPoints", m_syncPoints);
        deserializer.ReadValue("duration", m_duration);
    }
    else
    {
        std::array<nemesis::hkbGeneratorSyncInfoSyncPoint, 8> syncPoints;

        deserializer.ReadObject("syncPoints", syncPoints); // offset: 0/0 size: 64/64 align: 4/4

        for (size_t i = 0; i < syncPoints.size(); ++i)
        {
            m_syncPoints[i] = syncPoints[i];
        }

        deserializer.ReadValue("baseFrequency", m_baseFrequency); // offset: 64/64 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("localTime", m_localTime);         // offset: 68/68 size: 4/4 align: 4/4
    deserializer.ReadValue("playbackSpeed", m_playbackSpeed); // offset: 72/72 size: 4/4 align: 4/4
    deserializer.ReadValue("numSyncPoints", m_numSyncPoints); // offset: 76/76 size: 1/1 align: 1/1
    deserializer.ReadValue("isCyclic", m_isCyclic);           // offset: 77/77 size: 1/1 align: 1/1
    deserializer.ReadValue("isMirrored", m_isMirrored);       // offset: 78/78 size: 1/1 align: 1/1
    deserializer.ReadValue("isAdditive", m_isAdditive);       // offset: 79/79 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("activeInterval", m_activeInterval);
    }

    // class size: 80/80 align: 4/4
}

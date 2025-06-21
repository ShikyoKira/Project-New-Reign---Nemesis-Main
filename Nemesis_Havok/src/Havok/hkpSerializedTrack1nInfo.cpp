#include "Havok/hkpSerializedTrack1nInfo.h"
#include "Havok/hkpSerializedSubTrack1nInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedTrack1nInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedTrack1nInfo::Class; });

nemesis::hkpSerializedTrack1nInfo::hkpSerializedTrack1nInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

nemesis::hkpSerializedTrack1nInfo::hkpSerializedTrack1nInfo(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSerializedTrack1nInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedTrack1nInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("sectors", m_sectors);     // offset: 0/0 size: 12/16 align: 4/8
    serializer.WriteObject("subTracks", m_subTracks); // offset: 12/16 size: 12/16 align: 4/8
    // class size: 24/32 align: 4/8
}

void nemesis::hkpSerializedTrack1nInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("sectors", m_sectors);     // offset: 0/0 size: 12/16 align: 4/8
    deserializer.ReadObject("subTracks", m_subTracks); // offset: 12/16 size: 12/16 align: 4/8
    // class size: 24/32 align: 4/8
}

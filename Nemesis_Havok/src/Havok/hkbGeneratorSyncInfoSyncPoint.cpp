#include "Havok/hkbGeneratorSyncInfoSyncPoint.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorSyncInfoSyncPoint,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorSyncInfoSyncPoint::Class; });

nemesis::hkbGeneratorSyncInfoSyncPoint::hkbGeneratorSyncInfoSyncPoint() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbGeneratorSyncInfoSyncPoint::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorSyncInfoSyncPoint::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("id", m_id);     // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("time", m_time); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkbGeneratorSyncInfoSyncPoint::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("id", m_id);     // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("time", m_time); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

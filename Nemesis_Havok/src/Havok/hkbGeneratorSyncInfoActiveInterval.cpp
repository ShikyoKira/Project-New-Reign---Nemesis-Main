#include "Havok/hkbGeneratorSyncInfoActiveInterval.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGeneratorSyncInfoActiveInterval,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGeneratorSyncInfoActiveInterval::Class; });

nemesis::hkbGeneratorSyncInfoActiveInterval::hkbGeneratorSyncInfoActiveInterval() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbGeneratorSyncInfoActiveInterval::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGeneratorSyncInfoActiveInterval::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("syncPoints", m_syncPoints); // offset: 0/0 size: 16/16 align: 4/4
    serializer.WriteValue("fraction", m_fraction);      // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}

void nemesis::hkbGeneratorSyncInfoActiveInterval::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("syncPoints", m_syncPoints); // offset: 0/0 size: 16/16 align: 4/4
    deserializer.ReadValue("fraction", m_fraction);      // offset: 16/16 size: 4/4 align: 4/4
    // class size: 20/20 align: 4/4
}

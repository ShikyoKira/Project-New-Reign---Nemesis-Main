#include "Havok/hkbSenseHandleModifierRange.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSenseHandleModifierRange,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSenseHandleModifierRange::Class; });

nemesis::hkbSenseHandleModifierRange::hkbSenseHandleModifierRange() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbSenseHandleModifierRange::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSenseHandleModifierRange::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("event", m_event);              // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("minDistance", m_minDistance);   // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("maxDistance", m_maxDistance);   // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("ignoreHandle", m_ignoreHandle); // offset: 16/24 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());           // offset: 17/25 size: 3/7
    // class size: 20/32 align: 4/8
}

void nemesis::hkbSenseHandleModifierRange::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("event", m_event);              // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("minDistance", m_minDistance);   // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("maxDistance", m_maxDistance);   // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("ignoreHandle", m_ignoreHandle); // offset: 16/24 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());        // offset: 17/25 size: 3/7
    // class size: 20/32 align: 4/8
}

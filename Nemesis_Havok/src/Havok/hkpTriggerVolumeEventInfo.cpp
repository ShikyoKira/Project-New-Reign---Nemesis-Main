#include "Havok/hkpTriggerVolumeEventInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTriggerVolumeEventInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTriggerVolumeEventInfo::Class; });

nemesis::hkpTriggerVolumeEventInfo::hkpTriggerVolumeEventInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpTriggerVolumeEventInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTriggerVolumeEventInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("sortValue", m_sortValue); // offset: 0/0 size: 8/8 align: 8/8
    serializer.WriteObject("body", m_body);          // offset: 8/8 size: 4/8 align: 4/8
    serializer.WriteValue("operation", m_operation); // offset: 12/16 size: 4/4 align: 4/4
    serializer.Pad(8);                               // offset: 16/20 size: 0/4
    // class size: 16/24 align: 8/8
}

void nemesis::hkpTriggerVolumeEventInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("sortValue", m_sortValue); // offset: 0/0 size: 8/8 align: 8/8
    deserializer.ReadObject("body", m_body);          // offset: 8/8 size: 4/8 align: 4/8
    deserializer.ReadValue("operation", m_operation); // offset: 12/16 size: 4/4 align: 4/4
    deserializer.Pad(8);                              // offset: 16/20 size: 0/4
    // class size: 16/24 align: 8/8
}

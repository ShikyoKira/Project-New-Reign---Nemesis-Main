#include "Havok/hkbEventInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventInfo::Class; });

nemesis::hkbEventInfo::hkbEventInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbEventInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("flags", m_flags); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

void nemesis::hkbEventInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("flags", m_flags); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

#include "Havok/hkTraceStreamTitle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkTraceStreamTitle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkTraceStreamTitle::Class; });

nemesis::hkTraceStreamTitle::hkTraceStreamTitle() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkTraceStreamTitle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkTraceStreamTitle::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("value", m_value, true); // offset: 0/0 size: 32/32 align: 1/1
    // class size: 32/32 align: 1/1
}

void nemesis::hkTraceStreamTitle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("value", m_value, true); // offset: 0/0 size: 32/32 align: 1/1
    // class size: 32/32 align: 1/1
}

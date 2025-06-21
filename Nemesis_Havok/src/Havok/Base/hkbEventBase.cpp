#include "Havok/Base/hkbEventBase.h"

nemesis::hkbEventBase::hkbEventBase(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkbEventBase::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("id", m_id);           // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 4/4 size: 0/4

    serializer.WriteObject("payload", m_payload); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbEventBase::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("id", m_id);              // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 4/4 size: 0/4

    deserializer.ReadObject("payload", m_payload); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

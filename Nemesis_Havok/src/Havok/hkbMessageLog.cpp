#include "Havok/hkbMessageLog.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbMessageLog,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbMessageLog::Class; });

nemesis::hkbMessageLog::hkbMessageLog() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbMessageLog::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbMessageLog::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("messages", m_messages);      // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("maxMessages", m_maxMessages); // offset: 4/8 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                         // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkbMessageLog::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("messages", m_messages);      // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("maxMessages", m_maxMessages); // offset: 4/8 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

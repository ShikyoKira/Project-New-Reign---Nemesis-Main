#include "Havok/hkbClipTrigger.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClipTrigger,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClipTrigger::Class; });

nemesis::hkbClipTrigger::hkbClipTrigger() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbClipTrigger::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClipTrigger::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    serializer.WriteValue("localTime", m_localTime); // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                    // offset: 4/4 size: 0/4

    serializer.WriteObject("event", m_event);                            // offset: 4/8 size: 8/16 align: 4/8
    serializer.WriteValue("relativeToEndOfClip", m_relativeToEndOfClip); // offset: 12/24 size: 1/1 align: 1/1
    serializer.WriteValue("acyclic", m_acyclic);                         // offset: 13/25 size: 1/1 align: 1/1
    serializer.WriteValue("isAnnotation", m_isAnnotation);               // offset: 14/26 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                        // offset: 15/27 size: 1/5
    // class size: 16/32 align: 4/8
}

void nemesis::hkbClipTrigger::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    deserializer.ReadValue("localTime", m_localTime); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                   // offset: 4/4 size: 0/4

    deserializer.ReadObject("event", m_event); // offset: 4/8 size: 8/16 align: 4/8
    deserializer.ReadValue("relativeToEndOfClip",
                           m_relativeToEndOfClip);          // offset: 12/24 size: 1/1 align: 1/1
    deserializer.ReadValue("acyclic", m_acyclic);           // offset: 13/25 size: 1/1 align: 1/1
    deserializer.ReadValue("isAnnotation", m_isAnnotation); // offset: 14/26 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                         // offset: 15/27 size: 1/5
    // class size: 16/32 align: 4/8
}

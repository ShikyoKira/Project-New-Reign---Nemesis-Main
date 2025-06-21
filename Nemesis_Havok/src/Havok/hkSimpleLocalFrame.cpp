#include "Havok/hkSimpleLocalFrame.h"

REGISTER_HAVOK_POINTER_SOURCE(hkSimpleLocalFrame,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSimpleLocalFrame::Class; });

nemesis::hkSimpleLocalFrame::hkSimpleLocalFrame() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkLocalFrame(Class)
{
}

const nemesis::hkClass* nemesis::hkSimpleLocalFrame::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkSimpleLocalFrame::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkLocalFrame::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                             // offset: 8/16 size: 8/0

    serializer.WriteValue("transform", m_transform);      // offset: 16/16 size: 64/64 align: 16/16
    serializer.WriteObject("children", m_children);       // offset: 80/80 size: 12/16 align: 4/8
    serializer.WriteObject("parentFrame", m_parentFrame); // offset: 92/96 size: 4/8 align: 4/8
    serializer.WriteObject("group", m_group);             // offset: 96/104 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 100/112 size: 4/8 align: 4/8
    serializer.Skip(8);                                   // offset: 104/120 size: 8/8
    // class size: 112/128 align: 16/16
}

void nemesis::hkSimpleLocalFrame::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkLocalFrame::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                 // offset: 8/16 size: 8/0

    deserializer.ReadValue("transform", m_transform);      // offset: 16/16 size: 64/64 align: 16/16
    deserializer.ReadObject("children", m_children);       // offset: 80/80 size: 12/16 align: 4/8
    deserializer.ReadObject("parentFrame", m_parentFrame); // offset: 92/96 size: 4/8 align: 4/8
    deserializer.ReadObject("group", m_group);             // offset: 96/104 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);                // offset: 100/112 size: 4/8 align: 4/8
    deserializer.Skip(8);                                  // offset: 104/120 size: 8/8
    // class size: 112/128 align: 16/16
}

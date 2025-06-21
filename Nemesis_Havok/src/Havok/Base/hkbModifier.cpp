#include "Havok/Base/hkbModifier.h"

nemesis::hkbModifier::hkbModifier(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbNode(cls)
{
}

void nemesis::hkbModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbNode::SerializeTo(serializer);                           // offset: 0/0 size: 40/72 align: 4/8
    serializer.WriteValue("enable", m_enable);                           // offset: 40/72 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("padModifier", m_padModifier); // offset: 41/73 size: 3/3 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                         // offset: 44/76 size: 0/4
    // class size: 44/80 align: 4/8
}

void nemesis::hkbModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbNode::DeserializeFrom(deserializer);      // offset: 0/0 size: 40/72 align: 4/8
    deserializer.ReadValue("enable", m_enable);           // offset: 40/72 size: 1/1 align: 1/1
    deserializer.ReadValue("padModifier", m_padModifier); // offset: 41/73 size: 3/3 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 44/76 size: 0/4
    // class size: 44/80 align: 4/8
}

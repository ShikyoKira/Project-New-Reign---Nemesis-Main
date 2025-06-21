#include "Havok/Base/hkbNode.h"

nemesis::hkbNode::hkbNode(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbBindable(cls)
{
}

void nemesis::hkbNode::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer);                     // offset: 0/0 size: 28/48 align: 4/8
    serializer.WriteValue("userData", m_userData);                     // offset: 28/48 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);                             // offset: 32/56 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("id", m_id);                 // offset: 36/64 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("cloneState", m_cloneState); // offset: 38/66 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("padNode", m_padNode);       // offset: 39/67 size: 1/1 align: 1/1

    if (serializer.GetEndian() != 1 || serializer.GetPaddingOption() != 1)
    {
        serializer.Pad(serializer.GetPointerSize()); // offset: 40/68 size: 0/4
    }
    // class size: 40/72 align: 4/8
}

void nemesis::hkbNode::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer); // offset: 0/0 size: 28/48 align: 4/8
    deserializer.ReadValue("userData", m_userData);      // offset: 28/48 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);              // offset: 32/56 size: 4/8 align: 4/8
    deserializer.ReadValue("id", m_id);                  // offset: 36/64 size: 2/2 align: 2/2
    deserializer.ReadValue("cloneState", m_cloneState);  // offset: 38/66 size: 1/1 align: 1/1
    deserializer.ReadValue("padNode", m_padNode);        // offset: 39/67 size: 1/1 align: 1/1

    if (deserializer.GetEndian() != 1 || deserializer.GetPaddingOption() != 1)
    {
        deserializer.Pad(deserializer.GetPointerSize()); // offset: 40/68 size: 0/4
    }
    // class size: 40/72 align: 4/8
}

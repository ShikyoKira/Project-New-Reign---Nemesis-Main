#include "Havok/hkxVertexAnimation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexAnimation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexAnimation::Class; });

nemesis::hkxVertexAnimation::hkxVertexAnimation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexAnimation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexAnimation::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("time", m_time);                // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());          // offset: 12/20 size: 0/4

    serializer.WriteObject("vertData", m_vertData);             // offset: 12/24 size: 104/136 align: 4/8
    serializer.WriteObject("vertexIndexMap", m_vertexIndexMap); // offset: 116/160 size: 12/16 align: 4/8
    serializer.WriteObject("componentMap", m_componentMap);     // offset: 128/176 size: 12/16 align: 4/8
    // class size: 140/192 align: 4/8
}

void nemesis::hkxVertexAnimation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("time", m_time);                     // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 12/20 size: 0/4

    deserializer.ReadObject("vertData", m_vertData);             // offset: 12/24 size: 104/136 align: 4/8
    deserializer.ReadObject("vertexIndexMap", m_vertexIndexMap); // offset: 116/160 size: 12/16 align: 4/8
    deserializer.ReadObject("componentMap", m_componentMap);     // offset: 128/176 size: 12/16 align: 4/8
    // class size: 140/192 align: 4/8
}

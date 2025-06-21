#include "Havok/hkxIndexBuffer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxIndexBuffer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxIndexBuffer::Class; });

nemesis::hkxIndexBuffer::hkxIndexBuffer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxIndexBuffer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxIndexBuffer::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("indexType", m_indexType); // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());     // offset: 9/17 size: 3/7

    serializer.WriteObject("indices16", m_indices16);              // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteObject("indices32", m_indices32);              // offset: 24/40 size: 12/16 align: 4/8
    serializer.WriteValue("vertexBaseOffset", m_vertexBaseOffset); // offset: 36/56 size: 4/4 align: 4/4
    serializer.WriteValue("length", m_length);                     // offset: 40/60 size: 4/4 align: 4/4
    // class size: 44/64 align: 4/8
}

void nemesis::hkxIndexBuffer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("indexType", m_indexType); // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 9/17 size: 3/7

    deserializer.ReadObject("indices16", m_indices16);              // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadObject("indices32", m_indices32);              // offset: 24/40 size: 12/16 align: 4/8
    deserializer.ReadValue("vertexBaseOffset", m_vertexBaseOffset); // offset: 36/56 size: 4/4 align: 4/4
    deserializer.ReadValue("length", m_length);                     // offset: 40/60 size: 4/4 align: 4/4
    // class size: 44/64 align: 4/8
}

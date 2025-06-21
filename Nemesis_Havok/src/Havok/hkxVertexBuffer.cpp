#include "Havok/hkxVertexBuffer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexBuffer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexBuffer::Class; });

nemesis::hkxVertexBuffer::hkxVertexBuffer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexBuffer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexBuffer::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
        serializer.WriteObject("data", m_data);               // offset: 8/16 size: 84/104 align: 4/8
        serializer.WriteObject("desc", m_desc);               // offset: 92/120 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteSerializeIgnoredObject("vertexDataClass", m_vertexDataClass);
        serializer.WriteObject("vertexData", m_vertexData);
        serializer.WriteValue("numVertexData", m_numVertexData);
        serializer.WriteObject("vertexDesc", m_vertexDesc);
    }

    // class size: 104/136 align: 4/8
}

void nemesis::hkxVertexBuffer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
        deserializer.ReadObject("data", m_data);                    // offset: 8/16 size: 84/104 align: 4/8
        deserializer.ReadObject("desc", m_desc);                    // offset: 92/120 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("vertexDataClass", m_vertexDataClass);
        deserializer.ReadObject("vertexData", m_vertexData);
        deserializer.ReadValue("numVertexData", m_numVertexData);
        deserializer.ReadObject("vertexDesc", m_vertexDesc);
    }

    // class size: 104/136 align: 4/8
}

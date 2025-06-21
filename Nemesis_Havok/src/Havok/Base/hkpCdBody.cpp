#include "Havok/Base/hkpCdBody.h"

nemesis::hkpCdBody::hkpCdBody(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpCdBody::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("shape", m_shape);      // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("shapeKey", m_shapeKey); // offset: 4/8 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());   // offset: 8/12 size: 0/4

    serializer.WriteSerializeIgnoredObject("motion", m_motion); // offset: 8/16 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        serializer.WriteSerializeIgnoredObject("parent", m_parent); // offset: 12/24 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("parent", m_parent);
    }

    // class size: 16/32 align: 4/8
}

void nemesis::hkpCdBody::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("shape", m_shape);       // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("shapeKey", m_shapeKey);  // offset: 4/8 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 8/12 size: 0/4

    deserializer.ReadObject("motion", m_motion); // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("parent", m_parent); // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}

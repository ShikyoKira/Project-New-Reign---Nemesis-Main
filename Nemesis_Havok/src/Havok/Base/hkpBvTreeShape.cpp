#include "Havok/Base/hkpBvTreeShape.h"

nemesis::hkpBvTreeShape::hkpBvTreeShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpShape(cls)
{
}

void nemesis::hkpBvTreeShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteValue("bvTreeType", m_bvTreeType_1);
    }
    else
    {
        serializer.WriteValue("bvTreeType", m_bvTreeType_0); // offset: 16/32 size: 1/1 align: 1/1
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 17/33 size: 3/7
    // class size: 20/40 align: 4/8
}

void nemesis::hkpBvTreeShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("bvTreeType", m_bvTreeType_1);
        m_bvTreeType_0 = m_bvTreeType_1;
    }
    else
    {
        deserializer.ReadValue("bvTreeType", m_bvTreeType_0); // offset: 16/32 size: 1/1 align: 1/1
        m_bvTreeType_1 = m_bvTreeType_0;
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 17/33 size: 3/7
    // class size: 20/40 align: 4/8
}

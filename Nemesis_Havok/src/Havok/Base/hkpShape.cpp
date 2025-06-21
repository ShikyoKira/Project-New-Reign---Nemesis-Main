#include "Havok/Base/hkpShape.h"

nemesis::hkpShape::hkpShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpShapeBase(cls)
{
}

void nemesis::hkpShape::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_2_0)
    {
        nemesis::hkpShapeBase::SerializeTo(serializer);
        serializer.WriteValue("userData", m_userData);
        return;
    }

    nemesis::hkReferencedObject::SerializeTo(serializer);  // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("userData", m_userData);         // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("type", m_type); // offset: 12/24 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());           // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

void nemesis::hkpShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_2_0)
    {
        nemesis::hkpShapeBase::DeserializeFrom(deserializer);
        deserializer.ReadValue("userData", m_userData);
        return;
    }

    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("userData", m_userData);             // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);                     // offset: 12/24 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

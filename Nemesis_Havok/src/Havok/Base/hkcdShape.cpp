#include "Havok/Base/hkcdShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkcdShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkcdShape::Class; });

nemesis::hkcdShape::hkcdShape() noexcept
    : nemesis::hkcdShape(Class)
{
}

nemesis::hkcdShape::hkcdShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkcdShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);              // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("type", m_type);                             // offset: 8/16 size: 1/1 align: 1/1
    serializer.WriteValue("dispatchType", m_dispatchType);             // offset: 9/17 size: 1/1 align: 1/1
    serializer.WriteValue("bitsPerKey", m_bitsPerKey);                 // offset: 10/18 size: 1/1 align: 1/1
    serializer.WriteValue("shapeInfoCodecType", m_shapeInfoCodecType); // offset: 11/19 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkcdShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);         // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                             // offset: 8/16 size: 1/1 align: 1/1
    deserializer.ReadValue("dispatchType", m_dispatchType);             // offset: 9/17 size: 1/1 align: 1/1
    deserializer.ReadValue("bitsPerKey", m_bitsPerKey);                 // offset: 10/18 size: 1/1 align: 1/1
    deserializer.ReadValue("shapeInfoCodecType", m_shapeInfoCodecType); // offset: 11/19 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

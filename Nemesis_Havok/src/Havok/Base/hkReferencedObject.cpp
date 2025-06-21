#include "Havok/Base/hkReferencedObject.h"

nemesis::hkReferencedObject::hkReferencedObject(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkBaseObject(cls.GetSignature())
    , RefClass(&cls)
{
}

const nemesis::hkClass* nemesis::hkReferencedObject::GetClass(nemesis::HavokVersion version) const
{
    return RefClass;
}

void nemesis::hkReferencedObject::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkBaseObject::SerializeTo(serializer); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("memSizeAndFlags",
                                          m_memSizeAndFlags); // offset: 4/8 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("referenceCount",
                                          m_referenceCount); // offset: 6/10 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());             // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkReferencedObject::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkBaseObject::DeserializeFrom(deserializer);         // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("memSizeAndFlags", m_memSizeAndFlags); // offset: 4/8 size: 2/2 align: 2/2
    deserializer.ReadValue("referenceCount", m_referenceCount);   // offset: 6/10 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

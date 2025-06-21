#include "Havok/hkxAnimatedFloat.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxAnimatedFloat,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxAnimatedFloat::Class; });

nemesis::hkxAnimatedFloat::hkxAnimatedFloat() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxAnimatedFloat::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxAnimatedFloat::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("floats", m_floats);  // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("hint", m_hint);       // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkxAnimatedFloat::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("floats", m_floats);     // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("hint", m_hint);          // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

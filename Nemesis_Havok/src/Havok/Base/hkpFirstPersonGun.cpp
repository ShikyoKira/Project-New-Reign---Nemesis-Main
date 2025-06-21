#include "Havok/Base/hkpFirstPersonGun.h"

nemesis::hkpFirstPersonGun::hkpFirstPersonGun(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpFirstPersonGun::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);  // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("type", m_type); // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());           // offset: 9/17 size: 3/7

    serializer.WriteValue("name", m_name);               // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("keyboardKey", m_keyboardKey); // offset: 16/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());         // offset: 17/33 size: 3/7

    serializer.WriteSerializeIgnoredObject("listeners", m_listeners); // offset: 20/40 size: 12/16 align: 4/8
    // class size: 32/56 align: 4/8
}

void nemesis::hkpFirstPersonGun::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                     // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 9/17 size: 3/7

    deserializer.ReadValue("name", m_name);               // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("keyboardKey", m_keyboardKey); // offset: 16/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 17/33 size: 3/7

    deserializer.ReadObject("listeners", m_listeners); // offset: 20/40 size: 12/16 align: 4/8
    // class size: 32/56 align: 4/8
}

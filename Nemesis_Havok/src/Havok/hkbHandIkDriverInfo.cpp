#include "Havok/hkbHandIkDriverInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkDriverInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkDriverInfo::Class; });

nemesis::hkbHandIkDriverInfo::hkbHandIkDriverInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbHandIkDriverInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkDriverInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);      // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("hands", m_hands);                  // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("fadeInOutCurve", m_fadeInOutCurve); // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkbHandIkDriverInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("hands", m_hands);                  // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("fadeInOutCurve", m_fadeInOutCurve); // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

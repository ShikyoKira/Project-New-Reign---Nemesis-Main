#include "Havok/hkbHandIkModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkModifier::Class; });

nemesis::hkbHandIkModifier::hkbHandIkModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbHandIkModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);             // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("hands", m_hands);                  // offset: 44/80 size: 12/16 align: 4/8
    serializer.WriteValue("fadeInOutCurve", m_fadeInOutCurve); // offset: 56/96 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 57/97 size: 3/7

    serializer.WriteSerializeIgnoredObject("internalHandData",
                                           m_internalHandData); // offset: 60/104 size: 12/16 align: 4/8
    // class size: 72/120 align: 4/8
}

void nemesis::hkbHandIkModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);        // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("hands", m_hands);                  // offset: 44/80 size: 12/16 align: 4/8
    deserializer.ReadValue("fadeInOutCurve", m_fadeInOutCurve); // offset: 56/96 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 57/97 size: 3/7

    deserializer.ReadObject("internalHandData", m_internalHandData); // offset: 60/104 size: 12/16 align: 4/8
    // class size: 72/120 align: 4/8
}

#include "Havok/BSInterpValueModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSInterpValueModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSInterpValueModifier::Class; });

nemesis::BSInterpValueModifier::BSInterpValueModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSInterpValueModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSInterpValueModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("source", m_source);                     // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("target", m_target);                     // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteValue("result", m_result);                     // offset: 52/88 size: 4/4 align: 4/4
    serializer.WriteValue("gain", m_gain);                         // offset: 56/92 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep); // offset: 60/96 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 64/100 size: 0/4
    // class size: 64/104 align: 4/8
}

void nemesis::BSInterpValueModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("source", m_source);          // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("target", m_target);          // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadValue("result", m_result);          // offset: 52/88 size: 4/4 align: 4/4
    deserializer.ReadValue("gain", m_gain);              // offset: 56/92 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);      // offset: 60/96 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 64/100 size: 0/4
    // class size: 64/104 align: 4/8
}

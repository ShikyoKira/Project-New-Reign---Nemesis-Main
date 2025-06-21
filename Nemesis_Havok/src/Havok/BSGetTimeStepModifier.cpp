#include "Havok/BSGetTimeStepModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSGetTimeStepModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSGetTimeStepModifier::Class; });

nemesis::BSGetTimeStepModifier::BSGetTimeStepModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSGetTimeStepModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSGetTimeStepModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("timeStep", m_timeStep); // offset: 44/80 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());   // offset: 48/84 size: 0/4
    // class size: 48/88 align: 4/8
}

void nemesis::BSGetTimeStepModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("timeStep", m_timeStep);      // offset: 44/80 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 48/84 size: 0/4
    // class size: 48/88 align: 4/8
}

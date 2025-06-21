#include "Havok/hkbTransitionEffect.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTransitionEffect,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTransitionEffect::Class; });

nemesis::hkbTransitionEffect::hkbTransitionEffect() noexcept
    : nemesis::hkbTransitionEffect(Class)
{
}

nemesis::hkbTransitionEffect::hkbTransitionEffect(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbGenerator(cls)
{
}

const nemesis::hkClass* nemesis::hkbTransitionEffect::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTransitionEffect::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer);                    // offset: 0/0 size: 40/72 align: 4/8
    serializer.WriteValue("selfTransitionMode", m_selfTransitionMode); // offset: 40/72 size: 1/1 align: 1/1
    serializer.WriteValue("eventMode", m_eventMode);                   // offset: 41/73 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("defaultEventMode",
                                          m_defaultEventMode); // offset: 42/74 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 43/75 size: 1/5
    // class size: 44/80 align: 4/8
}

void nemesis::hkbTransitionEffect::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer);               // offset: 0/0 size: 40/72 align: 4/8
    deserializer.ReadValue("selfTransitionMode", m_selfTransitionMode); // offset: 40/72 size: 1/1 align: 1/1
    deserializer.ReadValue("eventMode", m_eventMode);                   // offset: 41/73 size: 1/1 align: 1/1
    deserializer.ReadValue("defaultEventMode", m_defaultEventMode);     // offset: 42/74 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 43/75 size: 1/5
    // class size: 44/80 align: 4/8
}

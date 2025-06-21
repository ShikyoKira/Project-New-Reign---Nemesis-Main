#include "Havok/hkbModifierGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbModifierGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbModifierGenerator::Class; });

nemesis::hkbModifierGenerator::hkbModifierGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbModifierGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbModifierGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(serializer.GetPointerSize());

    serializer.WriteObject("modifier", m_modifier);   // offset: 40/72 size: 4/8 align: 4/8
    serializer.WriteObject("generator", m_generator); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}

void nemesis::hkbModifierGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(deserializer.GetPointerSize());

    deserializer.ReadObject("modifier", m_modifier);   // offset: 40/72 size: 4/8 align: 4/8
    deserializer.ReadObject("generator", m_generator); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}

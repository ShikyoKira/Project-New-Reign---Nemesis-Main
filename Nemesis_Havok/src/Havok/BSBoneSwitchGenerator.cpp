#include "Havok/BSBoneSwitchGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BSBoneSwitchGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSBoneSwitchGenerator::Class; });

nemesis::BSBoneSwitchGenerator::BSBoneSwitchGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BSBoneSwitchGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSBoneSwitchGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(16);                             // offset: 40/72 size: 8/8

    serializer.WriteObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    serializer.WriteObject("ChildrenA", m_ChildrenA);                 // offset: 52/88 size: 12/16 align: 4/8
    serializer.Pad(16);                                               // offset: 64/104 size: 0/8
    // class size: 64/112 align: 16/16
}

void nemesis::BSBoneSwitchGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(16);                                 // offset: 40/72 size: 8/8

    deserializer.ReadObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.ReadObject("ChildrenA", m_ChildrenA);                 // offset: 52/88 size: 12/16 align: 4/8
    deserializer.Pad(16);                                              // offset: 64/104 size: 0/8
    // class size: 64/112 align: 16/16
}

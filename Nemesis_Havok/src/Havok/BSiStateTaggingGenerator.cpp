#include "Havok/BSiStateTaggingGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BSiStateTaggingGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSiStateTaggingGenerator::Class; });

nemesis::BSiStateTaggingGenerator::BSiStateTaggingGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BSiStateTaggingGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSiStateTaggingGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(16);                             // offset: 40/72 size: 8/8

    serializer.WriteObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    serializer.WriteValue("iStateToSetAs", m_iStateToSetAs);          // offset: 52/88 size: 4/4 align: 4/4
    serializer.WriteValue("iPriority", m_iPriority);                  // offset: 56/92 size: 4/4 align: 4/4
    serializer.Pad(16);                                               // offset: 60/96 size: 4/0
    // class size: 64/96 align: 16/16
}

void nemesis::BSiStateTaggingGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(16);                                 // offset: 40/72 size: 8/8

    deserializer.ReadObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.ReadValue("iStateToSetAs", m_iStateToSetAs);          // offset: 52/88 size: 4/4 align: 4/4
    deserializer.ReadValue("iPriority", m_iPriority);                  // offset: 56/92 size: 4/4 align: 4/4
    deserializer.Pad(16);                                              // offset: 60/96 size: 4/0
    // class size: 64/96 align: 16/16
}

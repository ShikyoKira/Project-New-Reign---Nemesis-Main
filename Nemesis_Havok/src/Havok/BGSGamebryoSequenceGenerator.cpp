#include "Havok/BGSGamebryoSequenceGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BGSGamebryoSequenceGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BGSGamebryoSequenceGenerator::Class; });

nemesis::BGSGamebryoSequenceGenerator::BGSGamebryoSequenceGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BGSGamebryoSequenceGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BGSGamebryoSequenceGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer);                    // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(serializer.GetPointerSize());

    serializer.WriteValue("pSequence", m_pSequence);                   // offset: 40/72 size: 4/8 align: 4/8
    serializer.WriteValue("eBlendModeFunction", m_eBlendModeFunction); // offset: 44/80 size: 1/1 align: 1/1
    serializer.Skip(3);                                                // offset: 45/81 size: 3/3

    serializer.WriteValue("fPercent", m_fPercent);              // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject("events", m_events); // offset: 52/88 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("fTime", m_fTime);    // offset: 64/104 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("bDelayedActivate",
                                          m_bDelayedActivate);     // offset: 68/108 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bLooping", m_bLooping); // offset: 69/109 size: 1/1 align: 1/1
    serializer.Skip(2);                                            // offset: 70/110 size: 2/2
    // class size: 72/112 align: 4/8
}

void nemesis::BGSGamebryoSequenceGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer);               // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(deserializer.GetPointerSize());

    deserializer.ReadValue("pSequence", m_pSequence);                   // offset: 40/72 size: 4/8 align: 4/8
    deserializer.ReadValue("eBlendModeFunction", m_eBlendModeFunction); // offset: 44/80 size: 1/1 align: 1/1
    deserializer.Skip(3);                                               // offset: 45/81 size: 3/3

    deserializer.ReadValue("fPercent", m_fPercent);                 // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadObject("events", m_events);                    // offset: 52/88 size: 12/16 align: 4/8
    deserializer.ReadValue("fTime", m_fTime);                       // offset: 64/104 size: 4/4 align: 4/4
    deserializer.ReadValue("bDelayedActivate", m_bDelayedActivate); // offset: 68/108 size: 1/1 align: 1/1
    deserializer.ReadValue("bLooping", m_bLooping);                 // offset: 69/109 size: 1/1 align: 1/1
    deserializer.Skip(2);                                           // offset: 70/110 size: 2/2
    // class size: 72/112 align: 4/8
}

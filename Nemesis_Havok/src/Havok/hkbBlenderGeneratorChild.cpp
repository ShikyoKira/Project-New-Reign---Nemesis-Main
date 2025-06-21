#include "Havok/hkbBlenderGeneratorChild.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlenderGeneratorChild,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlenderGeneratorChild::Class; });

nemesis::hkbBlenderGeneratorChild::hkbBlenderGeneratorChild() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::hkbBlenderGeneratorChild::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlenderGeneratorChild::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer); // offset: 0/0 size: 28/48 align: 4/8
    serializer.Pad(16);                            // offset: 28/48 size: 4/0

    serializer.WriteObject("generator", m_generator);     // offset: 32/48 size: 4/8 align: 16/16
    serializer.WriteObject("boneWeights", m_boneWeights); // offset: 36/56 size: 4/8 align: 4/8
    serializer.WriteValue("weight", m_weight);            // offset: 40/64 size: 4/4 align: 4/4
    serializer.WriteValue("worldFromModelWeight",
                          m_worldFromModelWeight); // offset: 44/68 size: 4/4 align: 4/4
    serializer.Pad(16);                            // offset: 48/72 size: 0/8
    // class size: 48/80 align: 16/16
}

void nemesis::hkbBlenderGeneratorChild::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer); // offset: 0/0 size: 28/48 align: 4/8
    deserializer.Pad(16);                                // offset: 28/48 size: 4/0

    deserializer.ReadObject("generator", m_generator);     // offset: 32/48 size: 4/8 align: 16/16
    deserializer.ReadObject("boneWeights", m_boneWeights); // offset: 36/56 size: 4/8 align: 4/8
    deserializer.ReadValue("weight", m_weight);            // offset: 40/64 size: 4/4 align: 4/4
    deserializer.ReadValue("worldFromModelWeight",
                           m_worldFromModelWeight); // offset: 44/68 size: 4/4 align: 4/4
    deserializer.Pad(16);                           // offset: 48/72 size: 0/8
    // class size: 48/80 align: 16/16
}

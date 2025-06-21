#include "Havok/BSBoneSwitchGeneratorBoneData.h"

REGISTER_HAVOK_POINTER_SOURCE(BSBoneSwitchGeneratorBoneData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSBoneSwitchGeneratorBoneData::Class; });

nemesis::BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::BSBoneSwitchGeneratorBoneData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSBoneSwitchGeneratorBoneData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer); // offset: 0/0 size: 28/48 align: 4/8
    serializer.Pad(16);                            // offset: 28/48 size: 4/0

    serializer.WriteObject("pGenerator", m_pGenerator);     // offset: 32/48 size: 4/8 align: 16/16
    serializer.WriteObject("spBoneWeight", m_spBoneWeight); // offset: 36/56 size: 4/8 align: 4/8
    serializer.Pad(16);                                     // offset: 40/64 size: 8/0
    // class size: 48/64 align: 16/16
}

void nemesis::BSBoneSwitchGeneratorBoneData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer); // offset: 0/0 size: 28/48 align: 4/8
    deserializer.Pad(16);                                // offset: 28/48 size: 4/0

    deserializer.ReadObject("pGenerator", m_pGenerator);     // offset: 32/48 size: 4/8 align: 16/16
    deserializer.ReadObject("spBoneWeight", m_spBoneWeight); // offset: 36/56 size: 4/8 align: 4/8
    deserializer.Pad(16);                                    // offset: 40/64 size: 8/0
    // class size: 48/64 align: 16/16
}

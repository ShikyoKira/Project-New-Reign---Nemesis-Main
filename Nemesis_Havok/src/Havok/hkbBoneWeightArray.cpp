#include "Havok/hkbBoneWeightArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBoneWeightArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBoneWeightArray::Class; });

nemesis::hkbBoneWeightArray::hkbBoneWeightArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::hkbBoneWeightArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBoneWeightArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer);        // offset: 0/0 size: 28/48 align: 4/8
    serializer.WriteObject("boneWeights", m_boneWeights); // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

void nemesis::hkbBoneWeightArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer);   // offset: 0/0 size: 28/48 align: 4/8
    deserializer.ReadObject("boneWeights", m_boneWeights); // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

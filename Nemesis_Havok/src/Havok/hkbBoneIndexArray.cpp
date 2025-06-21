#include "Havok/hkbBoneIndexArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBoneIndexArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBoneIndexArray::Class; });

nemesis::hkbBoneIndexArray::hkbBoneIndexArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::hkbBoneIndexArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBoneIndexArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer);        // offset: 0/0 size: 28/48 align: 4/8
    serializer.WriteObject("boneIndices", m_boneIndices); // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

void nemesis::hkbBoneIndexArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer);   // offset: 0/0 size: 28/48 align: 4/8
    deserializer.ReadObject("boneIndices", m_boneIndices); // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

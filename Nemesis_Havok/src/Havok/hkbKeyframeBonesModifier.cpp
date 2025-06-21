#include "Havok/hkbKeyframeBonesModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbKeyframeBonesModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbKeyframeBonesModifier::Class; });

nemesis::hkbKeyframeBonesModifier::hkbKeyframeBonesModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbKeyframeBonesModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbKeyframeBonesModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);          // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("keyframeInfo", m_keyframeInfo); // offset: 44/80 size: 12/16 align: 4/8
    serializer.WriteObject("keyframedBonesList", m_keyframedBonesList); // offset: 56/96 size: 4/8 align: 4/8
    // class size: 60/104 align: 4/8
}

void nemesis::hkbKeyframeBonesModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);     // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("keyframeInfo", m_keyframeInfo); // offset: 44/80 size: 12/16 align: 4/8
    deserializer.ReadObject("keyframedBonesList", m_keyframedBonesList); // offset: 56/96 size: 4/8 align: 4/8
    // class size: 60/104 align: 4/8
}

#include "Havok/hkbTwistModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTwistModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTwistModifier::Class; });

nemesis::hkbTwistModifier::hkbTwistModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbTwistModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTwistModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("axisOfRotation", m_axisOfRotation); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("twistAngle", m_twistAngle);         // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("startBoneIndex", m_startBoneIndex); // offset: 68/100 size: 2/2 align: 2/2
    serializer.WriteValue("endBoneIndex", m_endBoneIndex);     // offset: 70/102 size: 2/2 align: 2/2
    serializer.WriteValue("setAngleMethod", m_setAngleMethod); // offset: 72/104 size: 1/1 align: 1/1
    serializer.WriteValue("rotationAxisCoordinates",
                          m_rotationAxisCoordinates);  // offset: 73/105 size: 1/1 align: 1/1
    serializer.WriteValue("isAdditive", m_isAdditive); // offset: 74/106 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());       // offset: 75/107 size: 1/5

    serializer.WriteSerializeIgnoredObject("boneChainIndices",
                                           m_boneChainIndices); // offset: 76/112 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("parentBoneIndices",
                                           m_parentBoneIndices); // offset: 88/128 size: 12/16 align: 4/8
    serializer.Pad(16);                                          // offset: 100/144 size: 12/0
    // class size: 112/144 align: 16/16
}

void nemesis::hkbTwistModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("axisOfRotation", m_axisOfRotation); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("twistAngle", m_twistAngle);         // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("startBoneIndex", m_startBoneIndex); // offset: 68/100 size: 2/2 align: 2/2
    deserializer.ReadValue("endBoneIndex", m_endBoneIndex);     // offset: 70/102 size: 2/2 align: 2/2
    deserializer.ReadValue("setAngleMethod", m_setAngleMethod); // offset: 72/104 size: 1/1 align: 1/1
    deserializer.ReadValue("rotationAxisCoordinates",
                           m_rotationAxisCoordinates);  // offset: 73/105 size: 1/1 align: 1/1
    deserializer.ReadValue("isAdditive", m_isAdditive); // offset: 74/106 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());    // offset: 75/107 size: 1/5

    deserializer.ReadObject("boneChainIndices", m_boneChainIndices); // offset: 76/112 size: 12/16 align: 4/8
    deserializer.ReadObject("parentBoneIndices",
                            m_parentBoneIndices); // offset: 88/128 size: 12/16 align: 4/8
    deserializer.Pad(16);                         // offset: 100/144 size: 12/0
    // class size: 112/144 align: 16/16
}

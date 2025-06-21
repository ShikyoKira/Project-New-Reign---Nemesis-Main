#include "Havok/BSLimbIKModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSLimbIKModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSLimbIKModifier::Class; });

nemesis::BSLimbIKModifier::BSLimbIKModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSLimbIKModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSLimbIKModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("currentAngle",
                                          m_currentAngle);         // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteValue("startBoneIndex", m_startBoneIndex);     // offset: 52/88 size: 2/2 align: 2/2
    serializer.WriteValue("endBoneIndex", m_endBoneIndex);         // offset: 54/90 size: 2/2 align: 2/2
    serializer.WriteValue("gain", m_gain);                         // offset: 56/92 size: 4/4 align: 4/4
    serializer.WriteValue("boneRadius", m_boneRadius);             // offset: 60/96 size: 4/4 align: 4/4
    serializer.WriteValue("castOffset", m_castOffset);             // offset: 64/100 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep); // offset: 68/104 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 72/108 size: 0/4

    serializer.WriteSerializeIgnoredObject("pSkeletonMemory",
                                           m_pSkeletonMemory); // offset: 72/112 size: 4/8 align: 4/8
    // class size: 76/120 align: 4/8
}

void nemesis::BSLimbIKModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);              // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("currentAngle", m_currentAngle);           // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadValue("startBoneIndex", m_startBoneIndex);       // offset: 52/88 size: 2/2 align: 2/2
    deserializer.ReadValue("endBoneIndex", m_endBoneIndex);           // offset: 54/90 size: 2/2 align: 2/2
    deserializer.ReadValue("gain", m_gain);                           // offset: 56/92 size: 4/4 align: 4/4
    deserializer.ReadValue("boneRadius", m_boneRadius);               // offset: 60/96 size: 4/4 align: 4/4
    deserializer.ReadValue("castOffset", m_castOffset);               // offset: 64/100 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);                   // offset: 68/104 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 72/108 size: 0/4

    deserializer.ReadObject("pSkeletonMemory", m_pSkeletonMemory); // offset: 72/112 size: 4/8 align: 4/8
    // class size: 76/120 align: 4/8
}

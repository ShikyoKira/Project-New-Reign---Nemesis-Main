#include "Havok/hkbDetectCloseToGroundModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDetectCloseToGroundModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDetectCloseToGroundModifier::Class; });

nemesis::hkbDetectCloseToGroundModifier::hkbDetectCloseToGroundModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbDetectCloseToGroundModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDetectCloseToGroundModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                      // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("closeToGroundEvent", m_closeToGroundEvent); // offset: 44/80 size: 8/16 align: 4/8
    serializer.WriteValue("closeToGroundHeight", m_closeToGroundHeight); // offset: 52/96 size: 4/4 align: 4/4
    serializer.WriteValue("raycastDistanceDown",
                          m_raycastDistanceDown); // offset: 56/100 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo",
                          m_collisionFilterInfo);            // offset: 60/104 size: 4/4 align: 4/4
    serializer.WriteValue("boneIndex", m_boneIndex);         // offset: 64/108 size: 2/2 align: 2/2
    serializer.WriteValue("animBoneIndex", m_animBoneIndex); // offset: 66/110 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("isCloseToGround",
                                          m_isCloseToGround); // offset: 68/112 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());              // offset: 69/113 size: 3/7
    // class size: 72/120 align: 4/8
}

void nemesis::hkbDetectCloseToGroundModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("closeToGroundEvent",
                            m_closeToGroundEvent); // offset: 44/80 size: 8/16 align: 4/8
    deserializer.ReadValue("closeToGroundHeight",
                           m_closeToGroundHeight); // offset: 52/96 size: 4/4 align: 4/4
    deserializer.ReadValue("raycastDistanceDown",
                           m_raycastDistanceDown); // offset: 56/100 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo",
                           m_collisionFilterInfo);                // offset: 60/104 size: 4/4 align: 4/4
    deserializer.ReadValue("boneIndex", m_boneIndex);             // offset: 64/108 size: 2/2 align: 2/2
    deserializer.ReadValue("animBoneIndex", m_animBoneIndex);     // offset: 66/110 size: 2/2 align: 2/2
    deserializer.ReadValue("isCloseToGround", m_isCloseToGround); // offset: 68/112 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 69/113 size: 3/7
    // class size: 72/120 align: 4/8
}

#include "Havok/hkbExtractRagdollPoseModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbExtractRagdollPoseModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbExtractRagdollPoseModifier::Class; });

nemesis::hkbExtractRagdollPoseModifier::hkbExtractRagdollPoseModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbExtractRagdollPoseModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbExtractRagdollPoseModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("poseMatchingBone0", m_poseMatchingBone0); // offset: 44/80 size: 2/2 align: 2/2
    serializer.WriteValue("poseMatchingBone1", m_poseMatchingBone1); // offset: 46/82 size: 2/2 align: 2/2
    serializer.WriteValue("poseMatchingBone2", m_poseMatchingBone2); // offset: 48/84 size: 2/2 align: 2/2
    serializer.WriteValue("enableComputeWorldFromModel",
                          m_enableComputeWorldFromModel); // offset: 50/86 size: 1/1 align: 1/1
    serializer.Skip(1);                                   // offset: 51/87 size: 1/1
    // class size: 52/88 align: 4/8
}

void nemesis::hkbExtractRagdollPoseModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);              // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("poseMatchingBone0", m_poseMatchingBone0); // offset: 44/80 size: 2/2 align: 2/2
    deserializer.ReadValue("poseMatchingBone1", m_poseMatchingBone1); // offset: 46/82 size: 2/2 align: 2/2
    deserializer.ReadValue("poseMatchingBone2", m_poseMatchingBone2); // offset: 48/84 size: 2/2 align: 2/2
    deserializer.ReadValue("enableComputeWorldFromModel",
                           m_enableComputeWorldFromModel); // offset: 50/86 size: 1/1 align: 1/1
    deserializer.Skip(1);                                  // offset: 51/87 size: 1/1
    // class size: 52/88 align: 4/8
}

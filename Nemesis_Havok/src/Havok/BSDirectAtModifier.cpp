#include "Havok/BSDirectAtModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSDirectAtModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSDirectAtModifier::Class; });

nemesis::BSDirectAtModifier::BSDirectAtModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSDirectAtModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSDirectAtModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);             // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("directAtTarget", m_directAtTarget); // offset: 44/80 size: 1/1 align: 1/1
    serializer.Skip(1);                                        // offset: 45/81 size: 1/1

    serializer.WriteValue("sourceBoneIndex", m_sourceBoneIndex);         // offset: 46/82 size: 2/2 align: 2/2
    serializer.WriteValue("startBoneIndex", m_startBoneIndex);           // offset: 48/84 size: 2/2 align: 2/2
    serializer.WriteValue("endBoneIndex", m_endBoneIndex);               // offset: 50/86 size: 2/2 align: 2/2
    serializer.WriteValue("limitHeadingDegrees", m_limitHeadingDegrees); // offset: 52/88 size: 4/4 align: 4/4
    serializer.WriteValue("limitPitchDegrees", m_limitPitchDegrees);     // offset: 56/92 size: 4/4 align: 4/4
    serializer.WriteValue("offsetHeadingDegrees",
                          m_offsetHeadingDegrees);                     // offset: 60/96 size: 4/4 align: 4/4
    serializer.WriteValue("offsetPitchDegrees", m_offsetPitchDegrees); // offset: 64/100 size: 4/4 align: 4/4
    serializer.WriteValue("onGain", m_onGain);                         // offset: 68/104 size: 4/4 align: 4/4
    serializer.WriteValue("offGain", m_offGain);                       // offset: 72/108 size: 4/4 align: 4/4
    serializer.Pad(16);                                                // offset: 76/112 size: 4/0

    serializer.WriteValue("targetLocation", m_targetLocation); // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("userInfo", m_userInfo);             // offset: 96/128 size: 4/4 align: 4/4
    serializer.WriteValue("directAtCamera", m_directAtCamera); // offset: 100/132 size: 1/1 align: 1/1
    serializer.Skip(3);                                        // offset: 101/133 size: 3/3

    serializer.WriteValue("directAtCameraX", m_directAtCameraX); // offset: 104/136 size: 4/4 align: 4/4
    serializer.WriteValue("directAtCameraY", m_directAtCameraY); // offset: 108/140 size: 4/4 align: 4/4
    serializer.WriteValue("directAtCameraZ", m_directAtCameraZ); // offset: 112/144 size: 4/4 align: 4/4
    serializer.WriteValue("active", m_active);                   // offset: 116/148 size: 1/1 align: 1/1
    serializer.Skip(3);                                          // offset: 117/149 size: 3/3

    serializer.WriteValue("currentHeadingOffset",
                          m_currentHeadingOffset);                     // offset: 120/152 size: 4/4 align: 4/4
    serializer.WriteValue("currentPitchOffset", m_currentPitchOffset); // offset: 124/156 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep);     // offset: 128/160 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                       // offset: 132/164 size: 0/4

    serializer.WriteSerializeIgnoredObject("pSkeletonMemory",
                                           m_pSkeletonMemory);       // offset: 132/168 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("hasTarget", m_hasTarget); // offset: 136/176 size: 1/1 align: 1/1
    serializer.Pad(16);                                              // offset: 137/177 size: 7/15

    serializer.WriteSerializeIgnoredValue(
        "directAtTargetLocation", m_directAtTargetLocation); // offset: 144/192 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredObject("boneChainIndices",
                                           m_boneChainIndices); // offset: 160/208 size: 12/16 align: 4/8
    serializer.Pad(16);                                         // offset: 172/224 size: 4/0
    // class size: 176/224 align: 16/16
}

void nemesis::BSDirectAtModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);        // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("directAtTarget", m_directAtTarget); // offset: 44/80 size: 1/1 align: 1/1
    deserializer.Skip(1);                                       // offset: 45/81 size: 1/1

    deserializer.ReadValue("sourceBoneIndex", m_sourceBoneIndex); // offset: 46/82 size: 2/2 align: 2/2
    deserializer.ReadValue("startBoneIndex", m_startBoneIndex);   // offset: 48/84 size: 2/2 align: 2/2
    deserializer.ReadValue("endBoneIndex", m_endBoneIndex);       // offset: 50/86 size: 2/2 align: 2/2
    deserializer.ReadValue("limitHeadingDegrees",
                           m_limitHeadingDegrees);                    // offset: 52/88 size: 4/4 align: 4/4
    deserializer.ReadValue("limitPitchDegrees", m_limitPitchDegrees); // offset: 56/92 size: 4/4 align: 4/4
    deserializer.ReadValue("offsetHeadingDegrees",
                           m_offsetHeadingDegrees);                     // offset: 60/96 size: 4/4 align: 4/4
    deserializer.ReadValue("offsetPitchDegrees", m_offsetPitchDegrees); // offset: 64/100 size: 4/4 align: 4/4
    deserializer.ReadValue("onGain", m_onGain);                         // offset: 68/104 size: 4/4 align: 4/4
    deserializer.ReadValue("offGain", m_offGain);                       // offset: 72/108 size: 4/4 align: 4/4
    deserializer.Pad(16);                                               // offset: 76/112 size: 4/0

    deserializer.ReadValue("targetLocation", m_targetLocation); // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("userInfo", m_userInfo);             // offset: 96/128 size: 4/4 align: 4/4
    deserializer.ReadValue("directAtCamera", m_directAtCamera); // offset: 100/132 size: 1/1 align: 1/1
    deserializer.Skip(3);                                       // offset: 101/133 size: 3/3

    deserializer.ReadValue("directAtCameraX", m_directAtCameraX); // offset: 104/136 size: 4/4 align: 4/4
    deserializer.ReadValue("directAtCameraY", m_directAtCameraY); // offset: 108/140 size: 4/4 align: 4/4
    deserializer.ReadValue("directAtCameraZ", m_directAtCameraZ); // offset: 112/144 size: 4/4 align: 4/4
    deserializer.ReadValue("active", m_active);                   // offset: 116/148 size: 1/1 align: 1/1
    deserializer.Skip(3);                                         // offset: 117/149 size: 3/3

    deserializer.ReadValue("currentHeadingOffset",
                           m_currentHeadingOffset); // offset: 120/152 size: 4/4 align: 4/4
    deserializer.ReadValue("currentPitchOffset",
                           m_currentPitchOffset);    // offset: 124/156 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);  // offset: 128/160 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 132/164 size: 0/4

    deserializer.ReadObject("pSkeletonMemory", m_pSkeletonMemory); // offset: 132/168 size: 4/8 align: 4/8
    deserializer.ReadValue("hasTarget", m_hasTarget);              // offset: 136/176 size: 1/1 align: 1/1
    deserializer.Pad(16);                                          // offset: 137/177 size: 7/15

    deserializer.ReadValue("directAtTargetLocation",
                           m_directAtTargetLocation); // offset: 144/192 size: 16/16 align: 16/16
    deserializer.ReadObject("boneChainIndices", m_boneChainIndices); // offset: 160/208 size: 12/16 align: 4/8
    deserializer.Pad(16);                                            // offset: 172/224 size: 4/0
    // class size: 176/224 align: 16/16
}

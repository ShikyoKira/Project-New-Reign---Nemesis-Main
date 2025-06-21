#include "Havok/BSLookAtModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSLookAtModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSLookAtModifier::Class; });

nemesis::BSLookAtModifier::BSLookAtModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSLookAtModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

#include <iostream>

void nemesis::BSLookAtModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbModifier::SerializeTo(serializer);         // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("lookAtTarget", m_lookAtTarget); // offset: 44/80 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                          // offset: 45/81 size: 3/7

    serializer.WriteObject("bones", m_bones);                        // offset: 48/88 size: 12/16 align: 4/8
    serializer.WriteObject("eyeBones", m_eyeBones);                  // offset: 60/104 size: 12/16 align: 4/8
    serializer.WriteValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 72/120 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleThresholdDegrees",
                          m_limitAngleThresholdDegrees); // offset: 76/124 size: 4/4 align: 4/4
    serializer.WriteValue("continueLookOutsideOfLimit",
                          m_continueLookOutsideOfLimit); // offset: 80/128 size: 1/1 align: 1/1
    serializer.Skip(3);                                  // offset: 81/129 size: 3/3

    serializer.WriteValue("onGain", m_onGain);             // offset: 84/132 size: 4/4 align: 4/4
    serializer.WriteValue("offGain", m_offGain);           // offset: 88/136 size: 4/4 align: 4/4
    serializer.WriteValue("useBoneGains", m_useBoneGains); // offset: 92/140 size: 1/1 align: 1/1
    serializer.Skip(3);                                    // offset: 93/141 size: 3/3

    serializer.WriteValue("targetLocation", m_targetLocation); // offset: 96/144 size: 16/16 align: 16/16
    serializer.WriteValue("targetOutsideLimits",
                          m_targetOutsideLimits); // offset: 112/160 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                 // offset: 113/161 size: 3/7

    serializer.WriteObject("targetOutOfLimitEvent",
                           m_targetOutOfLimitEvent);       // offset: 116/168 size: 8/16 align: 4/8
    serializer.WriteValue("lookAtCamera", m_lookAtCamera); // offset: 124/184 size: 1/1 align: 1/1
    serializer.Skip(3);                                    // offset: 125/185 size: 3/3

    serializer.WriteValue("lookAtCameraX", m_lookAtCameraX);       // offset: 128/188 size: 4/4 align: 4/4
    serializer.WriteValue("lookAtCameraY", m_lookAtCameraY);       // offset: 132/192 size: 4/4 align: 4/4
    serializer.WriteValue("lookAtCameraZ", m_lookAtCameraZ);       // offset: 136/196 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep); // offset: 140/200 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("ballBonesValid",
                                          m_ballBonesValid); // offset: 144/204 size: 1/1 align: 1/1
    serializer.Skip(3);                                      // offset: 145/205 size: 3/3

    serializer.WriteSerializeIgnoredObject("pSkeletonMemory",
                                           m_pSkeletonMemory); // offset: 148/208 size: 4/8 align: 4/8
    serializer.Skip(8);                                        // offset: 152/216 size: 8/8
    // class size: 160/224 align: 16/16
}

void nemesis::BSLookAtModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbModifier::DeserializeFrom(deserializer);    // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("lookAtTarget", m_lookAtTarget); // offset: 44/80 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                         // offset: 45/81 size: 3/7

    deserializer.ReadObject("bones", m_bones);                        // offset: 48/88 size: 12/16 align: 4/8
    deserializer.ReadObject("eyeBones", m_eyeBones);                  // offset: 60/104 size: 12/16 align: 4/8
    deserializer.ReadValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 72/120 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleThresholdDegrees",
                           m_limitAngleThresholdDegrees); // offset: 76/124 size: 4/4 align: 4/4
    deserializer.ReadValue("continueLookOutsideOfLimit",
                           m_continueLookOutsideOfLimit); // offset: 80/128 size: 1/1 align: 1/1
    deserializer.Skip(3);                                 // offset: 81/129 size: 3/3

    deserializer.ReadValue("onGain", m_onGain);             // offset: 84/132 size: 4/4 align: 4/4
    deserializer.ReadValue("offGain", m_offGain);           // offset: 88/136 size: 4/4 align: 4/4
    deserializer.ReadValue("useBoneGains", m_useBoneGains); // offset: 92/140 size: 1/1 align: 1/1
    deserializer.Skip(3);                                   // offset: 93/141 size: 3/3

    deserializer.ReadValue("targetLocation", m_targetLocation); // offset: 96/144 size: 16/16 align: 16/16
    deserializer.ReadValue("targetOutsideLimits",
                           m_targetOutsideLimits); // offset: 112/160 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                // offset: 113/161 size: 3/7

    deserializer.ReadObject("targetOutOfLimitEvent",
                            m_targetOutOfLimitEvent);       // offset: 116/168 size: 8/16 align: 4/8
    deserializer.ReadValue("lookAtCamera", m_lookAtCamera); // offset: 124/184 size: 1/1 align: 1/1
    deserializer.Skip(3);                                   // offset: 125/185 size: 3/3

    deserializer.ReadValue("lookAtCameraX", m_lookAtCameraX);   // offset: 128/188 size: 4/4 align: 4/4
    deserializer.ReadValue("lookAtCameraY", m_lookAtCameraY);   // offset: 132/192 size: 4/4 align: 4/4
    deserializer.ReadValue("lookAtCameraZ", m_lookAtCameraZ);   // offset: 136/196 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);             // offset: 140/200 size: 4/4 align: 4/4
    deserializer.ReadValue("ballBonesValid", m_ballBonesValid); // offset: 144/204 size: 1/1 align: 1/1
    deserializer.Skip(3);                                       // offset: 145/205 size: 3/3

    deserializer.ReadObject("pSkeletonMemory", m_pSkeletonMemory); // offset: 148/208 size: 4/8 align: 4/8
    deserializer.Skip(8);                                          // offset: 152/216 size: 8/8
    // class size: 160/224 align: 16/16
}

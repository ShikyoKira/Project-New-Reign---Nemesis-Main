#include "Havok/hkbComputeRotationToTargetModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbComputeRotationToTargetModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbComputeRotationToTargetModifier::Class; });

nemesis::hkbComputeRotationToTargetModifier::hkbComputeRotationToTargetModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbComputeRotationToTargetModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbComputeRotationToTargetModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("rotationOut", m_rotationOut);         // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("targetPosition", m_targetPosition);   // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("currentPosition", m_currentPosition); // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("currentRotation", m_currentRotation); // offset: 96/128 size: 16/16 align: 16/16
    serializer.WriteValue("localAxisOfRotation",
                          m_localAxisOfRotation); // offset: 112/144 size: 16/16 align: 16/16
    serializer.WriteValue("localFacingDirection",
                          m_localFacingDirection);           // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("resultIsDelta", m_resultIsDelta); // offset: 144/176 size: 1/1 align: 1/1
    serializer.Skip(15);                                     // offset: 145/177 size: 15/15
    // class size: 160/192 align: 16/16
}

void nemesis::hkbComputeRotationToTargetModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("rotationOut", m_rotationOut);         // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("targetPosition", m_targetPosition);   // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("currentPosition", m_currentPosition); // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("currentRotation", m_currentRotation); // offset: 96/128 size: 16/16 align: 16/16
    deserializer.ReadValue("localAxisOfRotation",
                           m_localAxisOfRotation); // offset: 112/144 size: 16/16 align: 16/16
    deserializer.ReadValue("localFacingDirection",
                           m_localFacingDirection);           // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("resultIsDelta", m_resultIsDelta); // offset: 144/176 size: 1/1 align: 1/1
    deserializer.Skip(15);                                    // offset: 145/177 size: 15/15
    // class size: 160/192 align: 16/16
}

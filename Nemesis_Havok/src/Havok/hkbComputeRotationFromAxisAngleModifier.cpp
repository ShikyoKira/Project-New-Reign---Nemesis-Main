#include "Havok/hkbComputeRotationFromAxisAngleModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbComputeRotationFromAxisAngleModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbComputeRotationFromAxisAngleModifier::Class; });

nemesis::hkbComputeRotationFromAxisAngleModifier::hkbComputeRotationFromAxisAngleModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbComputeRotationFromAxisAngleModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbComputeRotationFromAxisAngleModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("rotationOut", m_rotationOut);   // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("axis", m_axis);                 // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("angleDegrees", m_angleDegrees); // offset: 80/112 size: 4/4 align: 4/4
    serializer.Skip(12);                                   // offset: 84/116 size: 12/12
    // class size: 96/128 align: 16/16
}

void nemesis::hkbComputeRotationFromAxisAngleModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("rotationOut", m_rotationOut);   // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("axis", m_axis);                 // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("angleDegrees", m_angleDegrees); // offset: 80/112 size: 4/4 align: 4/4
    deserializer.Skip(12);                                  // offset: 84/116 size: 12/12
    // class size: 96/128 align: 16/16
}

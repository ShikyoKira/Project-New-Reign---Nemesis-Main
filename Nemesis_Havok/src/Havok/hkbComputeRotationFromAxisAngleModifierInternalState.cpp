#include "Havok/hkbComputeRotationFromAxisAngleModifierInternalState.h"

nemesis::hkbComputeRotationFromAxisAngleModifierInternalState::
    hkbComputeRotationFromAxisAngleModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbComputeRotationFromAxisAngleModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbComputeRotationFromAxisAngleModifierInternalState::SerializeTo(
    nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("rotationOut", m_rotationOut); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkbComputeRotationFromAxisAngleModifierInternalState::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("rotationOut", m_rotationOut); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

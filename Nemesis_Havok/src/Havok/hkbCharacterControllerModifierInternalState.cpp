#include "Havok/hkbCharacterControllerModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControllerModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControllerModifierInternalState::Class; });

nemesis::hkbCharacterControllerModifierInternalState::hkbCharacterControllerModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterControllerModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControllerModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("gravity", m_gravity);   // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("timestep", m_timestep); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("isInitialVelocityAdded",
                          m_isInitialVelocityAdded);               // offset: 36/36 size: 1/1 align: 1/1
    serializer.WriteValue("isTouchingGround", m_isTouchingGround); // offset: 37/37 size: 1/1 align: 1/1
    serializer.Skip(10);                                           // offset: 38/38 size: 10/10
    // class size: 48/48 align: 16/16
}

void nemesis::hkbCharacterControllerModifierInternalState::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("gravity", m_gravity);   // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("timestep", m_timestep); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("isInitialVelocityAdded",
                           m_isInitialVelocityAdded);               // offset: 36/36 size: 1/1 align: 1/1
    deserializer.ReadValue("isTouchingGround", m_isTouchingGround); // offset: 37/37 size: 1/1 align: 1/1
    deserializer.Skip(10);                                          // offset: 38/38 size: 10/10
    // class size: 48/48 align: 16/16
}

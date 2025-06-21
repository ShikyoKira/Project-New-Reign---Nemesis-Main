#include "Havok/hkbRotateCharacterModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRotateCharacterModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRotateCharacterModifierInternalState::Class; });

nemesis::hkbRotateCharacterModifierInternalState::hkbRotateCharacterModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbRotateCharacterModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRotateCharacterModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("angle", m_angle);              // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());          // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkbRotateCharacterModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("angle", m_angle);                   // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

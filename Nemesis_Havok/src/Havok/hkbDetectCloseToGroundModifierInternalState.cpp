#include "Havok/hkbDetectCloseToGroundModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDetectCloseToGroundModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDetectCloseToGroundModifierInternalState::Class; });

nemesis::hkbDetectCloseToGroundModifierInternalState::hkbDetectCloseToGroundModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbDetectCloseToGroundModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDetectCloseToGroundModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);        // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("isCloseToGround", m_isCloseToGround); // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                 // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}

void nemesis::hkbDetectCloseToGroundModifierInternalState::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);   // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("isCloseToGround", m_isCloseToGround); // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}

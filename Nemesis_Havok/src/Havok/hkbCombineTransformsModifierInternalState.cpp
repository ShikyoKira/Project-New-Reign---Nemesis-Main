#include "Havok/hkbCombineTransformsModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCombineTransformsModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCombineTransformsModifierInternalState::Class; });

nemesis::hkbCombineTransformsModifierInternalState::hkbCombineTransformsModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCombineTransformsModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCombineTransformsModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("translationOut", m_translationOut); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("rotationOut", m_rotationOut);       // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}

void nemesis::hkbCombineTransformsModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("translationOut", m_translationOut); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("rotationOut", m_rotationOut);       // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}

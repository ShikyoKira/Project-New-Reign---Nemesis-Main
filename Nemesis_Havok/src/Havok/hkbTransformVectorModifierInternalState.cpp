#include "Havok/hkbTransformVectorModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTransformVectorModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTransformVectorModifierInternalState::Class; });

nemesis::hkbTransformVectorModifierInternalState::hkbTransformVectorModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbTransformVectorModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTransformVectorModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("vectorOut", m_vectorOut); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkbTransformVectorModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("vectorOut", m_vectorOut); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

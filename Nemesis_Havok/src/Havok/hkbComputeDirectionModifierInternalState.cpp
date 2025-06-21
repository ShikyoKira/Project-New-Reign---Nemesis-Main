#include "Havok/hkbComputeDirectionModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbComputeDirectionModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbComputeDirectionModifierInternalState::Class; });

nemesis::hkbComputeDirectionModifierInternalState::hkbComputeDirectionModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbComputeDirectionModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbComputeDirectionModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("pointOut", m_pointOut);             // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("groundAngleOut", m_groundAngleOut); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("upAngleOut", m_upAngleOut);         // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("computedOutput", m_computedOutput); // offset: 40/40 size: 1/1 align: 1/1
    serializer.Skip(7);                                        // offset: 41/41 size: 7/7
    // class size: 48/48 align: 16/16
}

void nemesis::hkbComputeDirectionModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("pointOut", m_pointOut);             // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("groundAngleOut", m_groundAngleOut); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("upAngleOut", m_upAngleOut);         // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("computedOutput", m_computedOutput); // offset: 40/40 size: 1/1 align: 1/1
    deserializer.Skip(7);                                       // offset: 41/41 size: 7/7
    // class size: 48/48 align: 16/16
}

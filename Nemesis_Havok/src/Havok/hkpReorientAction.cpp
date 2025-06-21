#include "Havok/hkpReorientAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpReorientAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpReorientAction::Class; });

nemesis::hkpReorientAction::hkpReorientAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpUnaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpReorientAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpReorientAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpUnaryAction::SerializeTo(serializer); // offset: 0/0 size: 28/56 align: 4/8
    serializer.Pad(16);                               // offset: 28/56 size: 4/8

    serializer.WriteValue("rotationAxis", m_rotationAxis); // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("upAxis", m_upAxis);             // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("strength", m_strength);         // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);           // offset: 68/100 size: 4/4 align: 4/4
    serializer.Skip(8);                                    // offset: 72/104 size: 8/8
    // class size: 80/112 align: 16/16
}

void nemesis::hkpReorientAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpUnaryAction::DeserializeFrom(deserializer); // offset: 0/0 size: 28/56 align: 4/8
    deserializer.Pad(16);                                   // offset: 28/56 size: 4/8

    deserializer.ReadValue("rotationAxis", m_rotationAxis); // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("upAxis", m_upAxis);             // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("strength", m_strength);         // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);           // offset: 68/100 size: 4/4 align: 4/4
    deserializer.Skip(8);                                   // offset: 72/104 size: 8/8
    // class size: 80/112 align: 16/16
}

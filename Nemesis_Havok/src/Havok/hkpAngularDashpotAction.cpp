#include "Havok/hkpAngularDashpotAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAngularDashpotAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAngularDashpotAction::Class; });

nemesis::hkpAngularDashpotAction::hkpAngularDashpotAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBinaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpAngularDashpotAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAngularDashpotAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBinaryAction::SerializeTo(serializer); // offset: 0/0 size: 32/64 align: 4/8
    serializer.WriteValue("rotation", m_rotation);     // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("strength", m_strength);     // offset: 48/80 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);       // offset: 52/84 size: 4/4 align: 4/4
    serializer.Skip(8);                                // offset: 56/88 size: 8/8
    // class size: 64/96 align: 16/16
}

void nemesis::hkpAngularDashpotAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBinaryAction::DeserializeFrom(deserializer); // offset: 0/0 size: 32/64 align: 4/8
    deserializer.ReadValue("rotation", m_rotation);          // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("strength", m_strength);          // offset: 48/80 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);            // offset: 52/84 size: 4/4 align: 4/4
    deserializer.Skip(8);                                    // offset: 56/88 size: 8/8
    // class size: 64/96 align: 16/16
}

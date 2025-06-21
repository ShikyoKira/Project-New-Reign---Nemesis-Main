#include "Havok/hkpDashpotAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDashpotAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDashpotAction::Class; });

nemesis::hkpDashpotAction::hkpDashpotAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBinaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpDashpotAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDashpotAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBinaryAction::SerializeTo(serializer); // offset: 0/0 size: 32/64 align: 4/8
    serializer.WriteValue("point", m_point);           // offset: 32/64 size: 32/32 align: 16/16
    serializer.WriteValue("strength", m_strength);     // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);       // offset: 68/100 size: 4/4 align: 4/4
    serializer.Skip(8);                                // offset: 72/104 size: 8/8

    serializer.WriteValue("impulse", m_impulse); // offset: 80/112 size: 16/16 align: 16/16
    // class size: 96/128 align: 16/16
}

void nemesis::hkpDashpotAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBinaryAction::DeserializeFrom(deserializer); // offset: 0/0 size: 32/64 align: 4/8
    deserializer.ReadValue("point", m_point);                // offset: 32/64 size: 32/32 align: 16/16
    deserializer.ReadValue("strength", m_strength);          // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping);            // offset: 68/100 size: 4/4 align: 4/4
    deserializer.Skip(8);                                    // offset: 72/104 size: 8/8

    deserializer.ReadValue("impulse", m_impulse); // offset: 80/112 size: 16/16 align: 16/16
    // class size: 96/128 align: 16/16
}

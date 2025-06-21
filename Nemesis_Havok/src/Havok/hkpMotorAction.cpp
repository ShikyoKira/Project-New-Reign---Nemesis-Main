#include "Havok/hkpMotorAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMotorAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMotorAction::Class; });

nemesis::hkpMotorAction::hkpMotorAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpUnaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpMotorAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMotorAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpUnaryAction::SerializeTo(serializer); // offset: 0/0 size: 28/56 align: 4/8
    serializer.Pad(16);                               // offset: 28/56 size: 4/8

    serializer.WriteValue("axis", m_axis);         // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("spinRate", m_spinRate); // offset: 48/80 size: 4/4 align: 4/4
    serializer.WriteValue("gain", m_gain);         // offset: 52/84 size: 4/4 align: 4/4
    serializer.WriteValue("active", m_active);     // offset: 56/88 size: 1/1 align: 1/1
    serializer.Skip(7);                            // offset: 57/89 size: 7/7
    // class size: 64/96 align: 16/16
}

void nemesis::hkpMotorAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpUnaryAction::DeserializeFrom(deserializer); // offset: 0/0 size: 28/56 align: 4/8
    deserializer.Pad(16);                                   // offset: 28/56 size: 4/8

    deserializer.ReadValue("axis", m_axis);         // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("spinRate", m_spinRate); // offset: 48/80 size: 4/4 align: 4/4
    deserializer.ReadValue("gain", m_gain);         // offset: 52/84 size: 4/4 align: 4/4
    deserializer.ReadValue("active", m_active);     // offset: 56/88 size: 1/1 align: 1/1
    deserializer.Skip(7);                           // offset: 57/89 size: 7/7
    // class size: 64/96 align: 16/16
}

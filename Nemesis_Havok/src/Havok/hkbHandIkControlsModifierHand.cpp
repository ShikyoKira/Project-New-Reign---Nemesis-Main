#include "Havok/hkbHandIkControlsModifierHand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkControlsModifierHand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkControlsModifierHand::Class; });

nemesis::hkbHandIkControlsModifierHand::hkbHandIkControlsModifierHand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbHandIkControlsModifierHand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkControlsModifierHand::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("controlData", m_controlData); // offset: 0/0 size: 80/96 align: 16/16
    serializer.WriteValue("handIndex", m_handIndex);      // offset: 80/96 size: 4/4 align: 4/4
    serializer.WriteValue("enable", m_enable);            // offset: 84/100 size: 1/1 align: 1/1
    serializer.Skip(11);                                  // offset: 85/101 size: 11/11
    // class size: 96/112 align: 16/16
}

void nemesis::hkbHandIkControlsModifierHand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("controlData", m_controlData); // offset: 0/0 size: 80/96 align: 16/16
    deserializer.ReadValue("handIndex", m_handIndex);      // offset: 80/96 size: 4/4 align: 4/4
    deserializer.ReadValue("enable", m_enable);            // offset: 84/100 size: 1/1 align: 1/1
    deserializer.Skip(11);                                 // offset: 85/101 size: 11/11
    // class size: 96/112 align: 16/16
}

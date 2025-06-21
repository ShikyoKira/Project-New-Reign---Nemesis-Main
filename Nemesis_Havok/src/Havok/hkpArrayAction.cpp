#include "Havok/hkpArrayAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpArrayAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpArrayAction::Class; });

nemesis::hkpArrayAction::hkpArrayAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpArrayAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpArrayAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpAction::SerializeTo(serializer);    // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("entities", m_entities); // offset: 24/48 size: 12/16 align: 4/8
    // class size: 36/64 align: 4/8
}

void nemesis::hkpArrayAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpAction::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("entities", m_entities);   // offset: 24/48 size: 12/16 align: 4/8
    // class size: 36/64 align: 4/8
}

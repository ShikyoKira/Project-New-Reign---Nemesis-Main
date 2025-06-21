#include "Havok/hkpConstraintChainInstance.h"
#include "Havok/hkpConstraintChainInstanceAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintChainInstanceAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintChainInstanceAction::Class; });

nemesis::hkpConstraintChainInstanceAction::hkpConstraintChainInstanceAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpConstraintChainInstanceAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintChainInstanceAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpAction::SerializeTo(serializer);                        // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("constraintInstance", m_constraintInstance); // offset: 24/48 size: 4/8 align: 4/8
    // class size: 28/56 align: 4/8
}

void nemesis::hkpConstraintChainInstanceAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpAction::DeserializeFrom(deserializer);                   // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("constraintInstance", m_constraintInstance); // offset: 24/48 size: 4/8 align: 4/8
    // class size: 28/56 align: 4/8
}

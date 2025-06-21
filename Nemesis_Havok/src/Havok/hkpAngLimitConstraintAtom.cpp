#include "Havok/hkpAngLimitConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAngLimitConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAngLimitConstraintAtom::Class; });

nemesis::hkpAngLimitConstraintAtom::hkpAngLimitConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpAngLimitConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAngLimitConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);     // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("limitAxis", m_limitAxis);     // offset: 3/3 size: 1/1 align: 1/1
    serializer.WriteValue("minAngle", m_minAngle);       // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("maxAngle", m_maxAngle);       // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("angularLimitsTauFactor",
                          m_angularLimitsTauFactor); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkpAngLimitConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);          // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("limitAxis", m_limitAxis);          // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("minAngle", m_minAngle);            // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAngle", m_maxAngle);            // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("angularLimitsTauFactor",
                           m_angularLimitsTauFactor); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

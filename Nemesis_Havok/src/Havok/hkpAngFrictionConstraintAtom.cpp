#include "Havok/hkpAngFrictionConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAngFrictionConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAngFrictionConstraintAtom::Class; });

nemesis::hkpAngFrictionConstraintAtom::hkpAngFrictionConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpAngFrictionConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAngFrictionConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);             // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);                 // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("firstFrictionAxis", m_firstFrictionAxis); // offset: 3/3 size: 1/1 align: 1/1
    serializer.WriteValue("numFrictionAxes", m_numFrictionAxes);     // offset: 4/4 size: 1/1 align: 1/1
    serializer.Skip(3);                                              // offset: 5/5 size: 3/3

    serializer.WriteValue("maxFrictionTorque", m_maxFrictionTorque); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

void nemesis::hkpAngFrictionConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);        // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);                 // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("firstFrictionAxis", m_firstFrictionAxis); // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("numFrictionAxes", m_numFrictionAxes);     // offset: 4/4 size: 1/1 align: 1/1
    deserializer.Skip(3);                                             // offset: 5/5 size: 3/3

    deserializer.ReadValue("maxFrictionTorque", m_maxFrictionTorque); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

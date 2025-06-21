#include "Havok/hkpAngConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAngConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAngConstraintAtom::Class; });

nemesis::hkpAngConstraintAtom::hkpAngConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpAngConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAngConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);                   // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("firstConstrainedAxis", m_firstConstrainedAxis); // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("numConstrainedAxes", m_numConstrainedAxes);     // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 2/2
}

void nemesis::hkpAngConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("firstConstrainedAxis",
                           m_firstConstrainedAxis);                     // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("numConstrainedAxes", m_numConstrainedAxes); // offset: 3/3 size: 1/1 align: 1/1
    // class size: 4/4 align: 2/2
}

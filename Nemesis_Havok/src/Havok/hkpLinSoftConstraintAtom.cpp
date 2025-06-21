#include "Havok/hkpLinSoftConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinSoftConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinSoftConstraintAtom::Class; });

nemesis::hkpLinSoftConstraintAtom::hkpLinSoftConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpLinSoftConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinSoftConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("axisIndex", m_axisIndex);     // offset: 2/2 size: 1/1 align: 1/1
    serializer.Skip(1);                                  // offset: 3/3 size: 1/1

    serializer.WriteValue("tau", m_tau);         // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

void nemesis::hkpLinSoftConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("axisIndex", m_axisIndex);          // offset: 2/2 size: 1/1 align: 1/1
    deserializer.Skip(1);                                      // offset: 3/3 size: 1/1

    deserializer.ReadValue("tau", m_tau);         // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping); // offset: 8/8 size: 4/4 align: 4/4
    // class size: 12/12 align: 4/4
}

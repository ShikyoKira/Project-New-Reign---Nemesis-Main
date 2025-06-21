#include "Havok/hkpOverwritePivotConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpOverwritePivotConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpOverwritePivotConstraintAtom::Class; });

nemesis::hkpOverwritePivotConstraintAtom::hkpOverwritePivotConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpOverwritePivotConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpOverwritePivotConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("copyToPivotBFromPivotA",
                          m_copyToPivotBFromPivotA); // offset: 2/2 size: 1/1 align: 1/1
    serializer.Skip(1);                              // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

void nemesis::hkpOverwritePivotConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("copyToPivotBFromPivotA",
                           m_copyToPivotBFromPivotA); // offset: 2/2 size: 1/1 align: 1/1
    deserializer.Skip(1);                             // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

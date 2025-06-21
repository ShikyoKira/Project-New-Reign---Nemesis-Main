#include "Havok/hkp2dAngConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkp2dAngConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkp2dAngConstraintAtom::Class; });

nemesis::hkp2dAngConstraintAtom::hkp2dAngConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkp2dAngConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkp2dAngConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);           // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("freeRotationAxis", m_freeRotationAxis); // offset: 2/2 size: 1/1 align: 1/1
    serializer.Skip(1);                                            // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

void nemesis::hkp2dAngConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);      // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("freeRotationAxis", m_freeRotationAxis); // offset: 2/2 size: 1/1 align: 1/1
    deserializer.Skip(1);                                           // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

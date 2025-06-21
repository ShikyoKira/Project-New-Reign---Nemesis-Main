#include "Havok/hkpViscousSurfaceModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpViscousSurfaceModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpViscousSurfaceModifierConstraintAtom::Class; });

nemesis::hkpViscousSurfaceModifierConstraintAtom::hkpViscousSurfaceModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpViscousSurfaceModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpViscousSurfaceModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    // class size: 32/48 align: 16/16
}

void nemesis::hkpViscousSurfaceModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    // class size: 32/48 align: 16/16
}

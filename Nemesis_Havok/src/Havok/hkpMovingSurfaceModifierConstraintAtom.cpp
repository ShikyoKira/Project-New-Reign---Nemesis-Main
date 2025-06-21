#include "Havok/hkpMovingSurfaceModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMovingSurfaceModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMovingSurfaceModifierConstraintAtom::Class; });

nemesis::hkpMovingSurfaceModifierConstraintAtom::hkpMovingSurfaceModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpMovingSurfaceModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMovingSurfaceModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    serializer.WriteValue("velocity", m_velocity);               // offset: 32/48 size: 16/16 align: 16/16
    // class size: 48/64 align: 16/16
}

void nemesis::hkpMovingSurfaceModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    deserializer.ReadValue("velocity", m_velocity); // offset: 32/48 size: 16/16 align: 16/16
    // class size: 48/64 align: 16/16
}

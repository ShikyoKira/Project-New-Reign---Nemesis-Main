#include "Havok/hkpIgnoreModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpIgnoreModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpIgnoreModifierConstraintAtom::Class; });

nemesis::hkpIgnoreModifierConstraintAtom::hkpIgnoreModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpIgnoreModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpIgnoreModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    // class size: 32/48 align: 16/16
}

void nemesis::hkpIgnoreModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    // class size: 32/48 align: 16/16
}

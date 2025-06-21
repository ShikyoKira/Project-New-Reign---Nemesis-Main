#include "Havok/hkpMassChangerModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMassChangerModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMassChangerModifierConstraintAtom::Class; });

nemesis::hkpMassChangerModifierConstraintAtom::hkpMassChangerModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpMassChangerModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMassChangerModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    serializer.WriteValue("factorA", m_factorA);                 // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("factorB", m_factorB);                 // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

void nemesis::hkpMassChangerModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    deserializer.ReadValue("factorA", m_factorA); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("factorB", m_factorB); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

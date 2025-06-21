#include "Havok/hkpCenterOfMassChangerModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCenterOfMassChangerModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCenterOfMassChangerModifierConstraintAtom::Class; });

nemesis::hkpCenterOfMassChangerModifierConstraintAtom::hkpCenterOfMassChangerModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpCenterOfMassChangerModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCenterOfMassChangerModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    serializer.WriteValue("displacementA", m_displacementA);     // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("displacementB", m_displacementB);     // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

void nemesis::hkpCenterOfMassChangerModifierConstraintAtom::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    deserializer.ReadValue("displacementA", m_displacementA); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("displacementB", m_displacementB); // offset: 48/64 size: 16/16 align: 16/16
    // class size: 64/80 align: 16/16
}

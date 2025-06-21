#include "Havok/hkpSoftContactModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSoftContactModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSoftContactModifierConstraintAtom::Class; });

nemesis::hkpSoftContactModifierConstraintAtom::hkpSoftContactModifierConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpModifierConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSoftContactModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSoftContactModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpModifierConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 16/16
    serializer.WriteValue("tau", m_tau);                         // offset: 32/48 size: 4/4 align: 4/4
    serializer.WriteValue("maxAcceleration", m_maxAcceleration); // offset: 36/52 size: 4/4 align: 4/4
    serializer.Skip(8);                                          // offset: 40/56 size: 8/8
    // class size: 48/64 align: 16/16
}

void nemesis::hkpSoftContactModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpModifierConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 16/16
    deserializer.ReadValue("tau", m_tau);                              // offset: 32/48 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAcceleration", m_maxAcceleration);      // offset: 36/52 size: 4/4 align: 4/4
    deserializer.Skip(8);                                              // offset: 40/56 size: 8/8
    // class size: 48/64 align: 16/16
}

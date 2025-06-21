#include "Havok/hkpPulleyConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPulleyConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPulleyConstraintAtom::Class; });

nemesis::hkpPulleyConstraintAtom::hkpPulleyConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpPulleyConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPulleyConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                                 // offset: 2/2 size: 14/14

    serializer.WriteValue("fixedPivotAinWorld",
                          m_fixedPivotAinWorld); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("fixedPivotBinWorld",
                          m_fixedPivotBinWorld);                 // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("ropeLength", m_ropeLength);           // offset: 48/48 size: 4/4 align: 4/4
    serializer.WriteValue("leverageOnBodyB", m_leverageOnBodyB); // offset: 52/52 size: 4/4 align: 4/4
    serializer.Skip(8);                                          // offset: 56/56 size: 8/8
    // class size: 64/64 align: 16/16
}

void nemesis::hkpPulleyConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                                     // offset: 2/2 size: 14/14

    deserializer.ReadValue("fixedPivotAinWorld",
                           m_fixedPivotAinWorld); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("fixedPivotBinWorld",
                           m_fixedPivotBinWorld);                 // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("ropeLength", m_ropeLength);           // offset: 48/48 size: 4/4 align: 4/4
    deserializer.ReadValue("leverageOnBodyB", m_leverageOnBodyB); // offset: 52/52 size: 4/4 align: 4/4
    deserializer.Skip(8);                                         // offset: 56/56 size: 8/8
    // class size: 64/64 align: 16/16
}

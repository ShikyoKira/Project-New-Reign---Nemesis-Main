#include "Havok/hkpGenericConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpGenericConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpGenericConstraintData::Class; });

nemesis::hkpGenericConstraintData::hkpGenericConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpGenericConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpGenericConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("atoms", m_atoms);            // offset: 12/24 size: 12/24 align: 4/8
    serializer.WriteObject("scheme", m_scheme);          // offset: 24/48 size: 64/80 align: 4/8
    // class size: 88/128 align: 4/8
}

void nemesis::hkpGenericConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                 // offset: 12/24 size: 12/24 align: 4/8
    deserializer.ReadObject("scheme", m_scheme);               // offset: 24/48 size: 64/80 align: 4/8
    // class size: 88/128 align: 4/8
}

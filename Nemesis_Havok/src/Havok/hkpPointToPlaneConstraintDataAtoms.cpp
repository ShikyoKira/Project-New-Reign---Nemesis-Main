#include "Havok/hkpPointToPlaneConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPointToPlaneConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPointToPlaneConstraintDataAtoms::Class; });

nemesis::hkpPointToPlaneConstraintDataAtoms::hkpPointToPlaneConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpPointToPlaneConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPointToPlaneConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    serializer.WriteObject("lin", m_lin);               // offset: 144/144 size: 4/4 align: 2/2
    serializer.Skip(12);                                // offset: 148/148 size: 12/12
    // class size: 160/160 align: 16/16
}

void nemesis::hkpPointToPlaneConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("transforms", m_transforms); // offset: 0/0 size: 144/144 align: 16/16
    deserializer.ReadObject("lin", m_lin);               // offset: 144/144 size: 4/4 align: 2/2
    deserializer.Skip(12);                               // offset: 148/148 size: 12/12
    // class size: 160/160 align: 16/16
}

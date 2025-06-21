#include "Havok/hkpPrismaticConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPrismaticConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPrismaticConstraintData::Class; });

nemesis::hkpPrismaticConstraintData::hkpPrismaticConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpPrismaticConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPrismaticConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.Pad(16);                                  // offset: 12/24 size: 4/8

    serializer.WriteObject("atoms", m_atoms); // offset: 16/32 size: 192/208 align: 16/16
    // class size: 208/240 align: 16/16
}

void nemesis::hkpPrismaticConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.Pad(16);                                      // offset: 12/24 size: 4/8

    deserializer.ReadObject("atoms", m_atoms); // offset: 16/32 size: 192/208 align: 16/16
    // class size: 208/240 align: 16/16
}

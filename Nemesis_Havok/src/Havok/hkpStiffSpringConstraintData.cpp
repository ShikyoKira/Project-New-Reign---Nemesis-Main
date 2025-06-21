#include "Havok/hkpStiffSpringConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStiffSpringConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStiffSpringConstraintData::Class; });

nemesis::hkpStiffSpringConstraintData::hkpStiffSpringConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpStiffSpringConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStiffSpringConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.Pad(16);                                  // offset: 12/24 size: 4/8

    serializer.WriteObject("atoms", m_atoms); // offset: 16/32 size: 64/64 align: 16/16
    // class size: 80/96 align: 16/16
}

void nemesis::hkpStiffSpringConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.Pad(16);                                      // offset: 12/24 size: 4/8

    deserializer.ReadObject("atoms", m_atoms); // offset: 16/32 size: 64/64 align: 16/16
    // class size: 80/96 align: 16/16
}

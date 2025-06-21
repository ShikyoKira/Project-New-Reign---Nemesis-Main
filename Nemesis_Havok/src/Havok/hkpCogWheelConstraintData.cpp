#include "Havok/hkpCogWheelConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCogWheelConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCogWheelConstraintData::Class; });

nemesis::hkpCogWheelConstraintData::hkpCogWheelConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpCogWheelConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCogWheelConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.Pad(16);                                  // offset: 12/24 size: 4/8

    serializer.WriteObject("atoms", m_atoms); // offset: 16/32 size: 160/160 align: 16/16
    // class size: 176/192 align: 16/16
}

void nemesis::hkpCogWheelConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.Pad(16);                                      // offset: 12/24 size: 4/8

    deserializer.ReadObject("atoms", m_atoms); // offset: 16/32 size: 160/160 align: 16/16
    // class size: 176/192 align: 16/16
}

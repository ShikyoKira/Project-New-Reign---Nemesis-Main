#include "Havok/hkpWheelConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWheelConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWheelConstraintData::Class; });

nemesis::hkpWheelConstraintData::hkpWheelConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpWheelConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWheelConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.Pad(16);                                  // offset: 12/24 size: 4/8

    serializer.WriteObject("atoms", m_atoms);                  // offset: 16/32 size: 304/304 align: 16/16
    serializer.WriteValue("initialAxleInB", m_initialAxleInB); // offset: 320/336 size: 16/16 align: 16/16
    serializer.WriteValue("initialSteeringAxisInB",
                          m_initialSteeringAxisInB); // offset: 336/352 size: 16/16 align: 16/16
    // class size: 352/368 align: 16/16
}

void nemesis::hkpWheelConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.Pad(16);                                      // offset: 12/24 size: 4/8

    deserializer.ReadObject("atoms", m_atoms);                  // offset: 16/32 size: 304/304 align: 16/16
    deserializer.ReadValue("initialAxleInB", m_initialAxleInB); // offset: 320/336 size: 16/16 align: 16/16
    deserializer.ReadValue("initialSteeringAxisInB",
                           m_initialSteeringAxisInB); // offset: 336/352 size: 16/16 align: 16/16
    // class size: 352/368 align: 16/16
}

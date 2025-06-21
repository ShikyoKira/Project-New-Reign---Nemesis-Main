#include "Havok/hkpPointToPathConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPointToPathConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPointToPathConstraintData::Class; });

nemesis::hkpPointToPathConstraintData::hkpPointToPathConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpPointToPathConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPointToPathConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer);           // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    serializer.WriteObject("path", m_path);                        // offset: 24/48 size: 4/8 align: 4/8
    serializer.WriteValue("maxFrictionForce", m_maxFrictionForce); // offset: 28/56 size: 4/4 align: 4/4
    serializer.WriteValue("angularConstrainedDOF",
                          m_angularConstrainedDOF); // offset: 32/60 size: 1/1 align: 1/1
    serializer.Pad(16);                             // offset: 33/61 size: 15/3

    serializer.WriteValue("transform_OS_KS", m_transform_OS_KS); // offset: 48/64 size: 128/128 align: 16/16
    // class size: 176/192 align: 16/16
}

void nemesis::hkpPointToPathConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer);      // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("atoms", m_atoms);                      // offset: 12/24 size: 12/24 align: 4/8
    deserializer.ReadObject("path", m_path);                        // offset: 24/48 size: 4/8 align: 4/8
    deserializer.ReadValue("maxFrictionForce", m_maxFrictionForce); // offset: 28/56 size: 4/4 align: 4/4
    deserializer.ReadValue("angularConstrainedDOF",
                           m_angularConstrainedDOF); // offset: 32/60 size: 1/1 align: 1/1
    deserializer.Pad(16);                            // offset: 33/61 size: 15/3

    deserializer.ReadValue("transform_OS_KS", m_transform_OS_KS); // offset: 48/64 size: 128/128 align: 16/16
    // class size: 176/192 align: 16/16
}

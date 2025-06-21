#include "Havok/hkpMalleableConstraintData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMalleableConstraintData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMalleableConstraintData::Class; });

nemesis::hkpMalleableConstraintData::hkpMalleableConstraintData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpWrappedConstraintData(Class)
{
}

const nemesis::hkClass* nemesis::hkpMalleableConstraintData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMalleableConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        nemesis::hkpWrappedConstraintData::SerializeTo(serializer);
    }
    else
    {
        nemesis::hkpConstraintData::SerializeTo(serializer);        // offset: 0/0 size: 12/24 align: 4/8
        serializer.WriteObject("constraintData", m_constraintData); // offset: 12/24 size: 4/8 align: 4/8
    }

    serializer.WriteObject("atoms", m_atoms);      // offset: 16/32 size: 12/24 align: 4/8
    serializer.WriteValue("strength", m_strength); // offset: 28/56 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());   // offset: 32/60 size: 0/4
    // class size: 32/64 align: 4/8
}

void nemesis::hkpMalleableConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        nemesis::hkpWrappedConstraintData::DeserializeFrom(deserializer);
    }
    else
    {
        nemesis::hkpConstraintData::DeserializeFrom(deserializer);   // offset: 0/0 size: 12/24 align: 4/8
        deserializer.ReadObject("constraintData", m_constraintData); // offset: 12/24 size: 4/8 align: 4/8
    }

    deserializer.ReadObject("atoms", m_atoms);       // offset: 16/32 size: 12/24 align: 4/8
    deserializer.ReadValue("strength", m_strength);  // offset: 28/56 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 32/60 size: 0/4
    // class size: 32/64 align: 4/8
}

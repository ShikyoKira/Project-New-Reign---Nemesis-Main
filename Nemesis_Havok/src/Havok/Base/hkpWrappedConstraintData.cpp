#include "Havok/Base/hkpWrappedConstraintData.h"

nemesis::hkpWrappedConstraintData::hkpWrappedConstraintData(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpConstraintData(cls)
{
}

void nemesis::hkpWrappedConstraintData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer);        // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteObject("constraintData", m_constraintData); // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}

void nemesis::hkpWrappedConstraintData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer);   // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadObject("constraintData", m_constraintData); // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}

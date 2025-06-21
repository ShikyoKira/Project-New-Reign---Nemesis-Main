#include "Havok/Base/hkpConstraintChainData.h"

nemesis::hkpConstraintChainData::hkpConstraintChainData(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpConstraintData(cls)
{
}

void nemesis::hkpConstraintChainData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintData::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkpConstraintChainData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintData::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    // class size: 12/24 align: 4/8
}

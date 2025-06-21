#include "Havok/Base/hkpShapeCollectionFilter.h"

nemesis::hkpShapeCollectionFilter::hkpShapeCollectionFilter(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpShapeCollectionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpShapeCollectionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

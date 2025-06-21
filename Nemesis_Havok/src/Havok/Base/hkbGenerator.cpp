#include "Havok/Base/hkbGenerator.h"

nemesis::hkbGenerator::hkbGenerator(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbNode(cls)
{
}

void nemesis::hkbGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbNode::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    // class size: 40/72 align: 4/8
}

void nemesis::hkbGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbNode::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    // class size: 40/72 align: 4/8
}

#include "Havok/Base/hkpContactListener.h"

nemesis::hkpContactListener::hkpContactListener(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpContactListener::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpContactListener::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

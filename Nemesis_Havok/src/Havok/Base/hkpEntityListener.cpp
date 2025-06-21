#include "Havok/Base/hkpEntityListener.h"

nemesis::hkpEntityListener::hkpEntityListener(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpEntityListener::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpEntityListener::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

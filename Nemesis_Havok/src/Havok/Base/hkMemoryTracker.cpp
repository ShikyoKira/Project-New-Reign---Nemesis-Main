#include "Havok/Base/hkMemoryTracker.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMemoryTracker,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMemoryTracker::Class; });

nemesis::hkMemoryTracker::hkMemoryTracker() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

void nemesis::hkMemoryTracker::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkMemoryTracker::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

#include "Havok/hkpDefaultWorldMemoryWatchDog.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDefaultWorldMemoryWatchDog,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDefaultWorldMemoryWatchDog::Class; });

nemesis::hkpDefaultWorldMemoryWatchDog::hkpDefaultWorldMemoryWatchDog() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkWorldMemoryAvailableWatchDog(Class)
{
}

const nemesis::hkClass* nemesis::hkpDefaultWorldMemoryWatchDog::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDefaultWorldMemoryWatchDog::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkWorldMemoryAvailableWatchDog::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_660) return;

    serializer.WriteValue("freeHeapMemoryRequested",
                          m_freeHeapMemoryRequested); // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());      // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkpDefaultWorldMemoryWatchDog::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkWorldMemoryAvailableWatchDog::DeserializeFrom(
        deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_660) return;

    deserializer.ReadValue("freeHeapMemoryRequested",
                           m_freeHeapMemoryRequested); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());   // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

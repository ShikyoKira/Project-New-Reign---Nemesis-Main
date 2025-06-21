#include "Havok/Base/hkWorldMemoryAvailableWatchDog.h"

nemesis::hkWorldMemoryAvailableWatchDog::hkWorldMemoryAvailableWatchDog(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkWorldMemoryAvailableWatchDog::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        serializer.WriteValue("minMemoryAvailable", m_minMemoryAvailable);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

void nemesis::hkWorldMemoryAvailableWatchDog::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        deserializer.ReadValue("minMemoryAvailable", m_minMemoryAvailable);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

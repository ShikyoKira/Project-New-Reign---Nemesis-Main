#include "Havok/hkMemoryResourceHandle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMemoryResourceHandle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMemoryResourceHandle::Class; });

nemesis::hkMemoryResourceHandle::hkMemoryResourceHandle() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkResourceHandle(Class)
{
}

const nemesis::hkClass* nemesis::hkMemoryResourceHandle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMemoryResourceHandle::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkResourceHandle::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("variant", m_variant);       // offset: 8/16 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        serializer.WriteValue("objectIsRerencedObject", m_objectIsRerencedObject);
        serializer.WriteValue("nameIsAllocated", m_nameIsAllocated);
        serializer.Pad(serializer.GetPointerSize());
    }

    serializer.WriteValue("name", m_name);              // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("references", m_references); // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}

void nemesis::hkMemoryResourceHandle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkResourceHandle::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("variant", m_variant);            // offset: 8/16 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        deserializer.ReadValue("objectIsRerencedObject", m_objectIsRerencedObject);
        deserializer.ReadValue("nameIsAllocated", m_nameIsAllocated);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    deserializer.ReadValue("name", m_name);                   // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("references", m_references);      // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}

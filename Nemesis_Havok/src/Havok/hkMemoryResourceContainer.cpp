#include "Havok/hkMemoryResourceContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMemoryResourceContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMemoryResourceContainer::Class; });

nemesis::hkMemoryResourceContainer::hkMemoryResourceContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkResourceContainer(Class)
{
}

const nemesis::hkClass* nemesis::hkMemoryResourceContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMemoryResourceContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkResourceContainer::SerializeTo(serializer);        // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() == HavokVersion::HK_660)
    {
        serializer.WriteValue("nameIsAllocated", m_nameIsAllocated);
        serializer.Pad(serializer.GetPointerSize());
    }

    serializer.WriteValue("name", m_name);                        // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("parent", m_parent);   // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("resourceHandles", m_resourceHandles); // offset: 16/32 size: 12/16 align: 4/8
    serializer.WriteObject("children", m_children);               // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

void nemesis::hkMemoryResourceContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkResourceContainer::DeserializeFrom(deserializer);   // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() == HavokVersion::HK_660)
    {
        deserializer.ReadValue("nameIsAllocated", m_nameIsAllocated);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    deserializer.ReadValue("name", m_name);                        // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("parent", m_parent);                   // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("resourceHandles", m_resourceHandles); // offset: 16/32 size: 12/16 align: 4/8
    deserializer.ReadObject("children", m_children);               // offset: 28/48 size: 12/16 align: 4/8
    // class size: 40/64 align: 4/8
}

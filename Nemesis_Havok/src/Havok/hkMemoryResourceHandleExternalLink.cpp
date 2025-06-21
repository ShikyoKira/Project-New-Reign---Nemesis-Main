#include "Havok/hkMemoryResourceHandleExternalLink.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMemoryResourceHandleExternalLink,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMemoryResourceHandleExternalLink::Class; });

nemesis::hkMemoryResourceHandleExternalLink::hkMemoryResourceHandleExternalLink() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkMemoryResourceHandleExternalLink::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMemoryResourceHandleExternalLink::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("memberName", m_memberName); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("externalId", m_externalId); // offset: 4/8 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        serializer.WriteValue("externalIdIsAllocated", m_externalIdIsAllocated);
        serializer.WriteValue("memberNameIsAllocated", m_memberNameIsAllocated);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

void nemesis::hkMemoryResourceHandleExternalLink::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("memberName", m_memberName); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("externalId", m_externalId); // offset: 4/8 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_660)
    {
        deserializer.ReadValue("externalIdIsAllocated", m_externalIdIsAllocated);
        deserializer.ReadValue("memberNameIsAllocated", m_memberNameIsAllocated);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

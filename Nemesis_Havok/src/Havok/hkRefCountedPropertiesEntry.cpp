#include "Havok/hkRefCountedPropertiesEntry.h"

REGISTER_HAVOK_POINTER_SOURCE(hkRefCountedPropertiesEntry,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkRefCountedPropertiesEntry::Class; });

nemesis::hkRefCountedPropertiesEntry::hkRefCountedPropertiesEntry() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkRefCountedPropertiesEntry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkRefCountedPropertiesEntry::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("object", m_object);  // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("key", m_key);         // offset: 4/8 size: 2/2 align: 2/2
    serializer.WriteValue("flags", m_flags);     // offset: 6/10 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize()); // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkRefCountedPropertiesEntry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("object", m_object);     // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("key", m_key);            // offset: 4/8 size: 2/2 align: 2/2
    deserializer.ReadValue("flags", m_flags);        // offset: 6/10 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

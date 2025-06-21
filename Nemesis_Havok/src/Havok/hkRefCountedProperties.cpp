#include "Havok/hkRefCountedProperties.h"

REGISTER_HAVOK_POINTER_SOURCE(hkRefCountedProperties,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkRefCountedProperties::Class; });

nemesis::hkRefCountedProperties::hkRefCountedProperties() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkRefCountedProperties::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkRefCountedProperties::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() <= HavokVersion::HK_2012_2_0)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("entries", m_entries); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkRefCountedProperties::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2012_2_0)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("entries", m_entries); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

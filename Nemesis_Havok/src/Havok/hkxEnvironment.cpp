#include "Havok/hkxEnvironment.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxEnvironment,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxEnvironment::Class; });

nemesis::hkxEnvironment::hkxEnvironment() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxEnvironment::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxEnvironment::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("variables", m_variables); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxEnvironment::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("variables", m_variables); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

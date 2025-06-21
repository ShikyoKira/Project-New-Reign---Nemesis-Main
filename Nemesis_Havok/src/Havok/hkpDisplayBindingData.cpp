#include "Havok/hkpDisplayBindingData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDisplayBindingData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDisplayBindingData::Class; });

nemesis::hkpDisplayBindingData::hkpDisplayBindingData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpDisplayBindingData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDisplayBindingData::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("rigidBodyBindings", m_rigidBodyBindings); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("physicsSystemBindings",
                           m_physicsSystemBindings); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkpDisplayBindingData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("rigidBodyBindings", m_rigidBodyBindings); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("physicsSystemBindings",
                            m_physicsSystemBindings); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

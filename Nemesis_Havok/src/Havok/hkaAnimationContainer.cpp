#include "Havok/hkaAnimationContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaAnimationContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaAnimationContainer::Class; });

nemesis::hkaAnimationContainer::hkaAnimationContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaAnimationContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaAnimationContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("skeletons", m_skeletons);     // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("animations", m_animations);   // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("bindings", m_bindings);       // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("attachments", m_attachments); // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteObject("skins", m_skins);             // offset: 56/80 size: 12/16 align: 4/8
    // class size: 68/96 align: 4/8
}

void nemesis::hkaAnimationContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("skeletons", m_skeletons);          // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("animations", m_animations);        // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("bindings", m_bindings);            // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("attachments", m_attachments);      // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadObject("skins", m_skins);                  // offset: 56/80 size: 12/16 align: 4/8
    // class size: 68/96 align: 4/8
}

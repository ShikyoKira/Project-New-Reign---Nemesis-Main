#include "Havok/hkxVertexSelectionChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexSelectionChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexSelectionChannel::Class; });

nemesis::hkxVertexSelectionChannel::hkxVertexSelectionChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexSelectionChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexSelectionChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("selectedVertices", m_selectedVertices); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxVertexSelectionChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("selectedVertices", m_selectedVertices); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

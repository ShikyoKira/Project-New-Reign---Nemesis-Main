#include "Havok/hkxEdgeSelectionChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxEdgeSelectionChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxEdgeSelectionChannel::Class; });

nemesis::hkxEdgeSelectionChannel::hkxEdgeSelectionChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxEdgeSelectionChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxEdgeSelectionChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("selectedEdges", m_selectedEdges); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxEdgeSelectionChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("selectedEdges", m_selectedEdges); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

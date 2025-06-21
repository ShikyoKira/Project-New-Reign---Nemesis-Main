#include "Havok/hkxTriangleSelectionChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxTriangleSelectionChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxTriangleSelectionChannel::Class; });

nemesis::hkxTriangleSelectionChannel::hkxTriangleSelectionChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxTriangleSelectionChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxTriangleSelectionChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("selectedTriangles", m_selectedTriangles); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxTriangleSelectionChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("selectedTriangles", m_selectedTriangles); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

#include "Havok/hkxVertexIntDataChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexIntDataChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexIntDataChannel::Class; });

nemesis::hkxVertexIntDataChannel::hkxVertexIntDataChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexIntDataChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexIntDataChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("perVertexInts", m_perVertexInts); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxVertexIntDataChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("perVertexInts", m_perVertexInts); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

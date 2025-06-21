#include "Havok/hkxVertexFloatDataChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexFloatDataChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexFloatDataChannel::Class; });

nemesis::hkxVertexFloatDataChannel::hkxVertexFloatDataChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexFloatDataChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexFloatDataChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("perVertexFloats", m_perVertexFloats); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("dimensions", m_dimensions);            // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                  // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkxVertexFloatDataChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("perVertexFloats", m_perVertexFloats); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("dimensions", m_dimensions);            // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());               // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

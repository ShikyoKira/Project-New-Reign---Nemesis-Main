#include "Havok/hkpCompressedSampledHeightFieldShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCompressedSampledHeightFieldShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCompressedSampledHeightFieldShape::Class; });

nemesis::hkpCompressedSampledHeightFieldShape::hkpCompressedSampledHeightFieldShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpSampledHeightFieldShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpCompressedSampledHeightFieldShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCompressedSampledHeightFieldShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpSampledHeightFieldShape::SerializeTo(serializer); // offset: 0/0 size: 96/112 align: 16/16
    serializer.WriteObject("storage", m_storage);                 // offset: 96/112 size: 12/16 align: 4/8
    serializer.WriteValue("triangleFlip", m_triangleFlip);        // offset: 108/128 size: 1/1 align: 1/1
    serializer.Skip(3);                                           // offset: 109/129 size: 3/3

    serializer.WriteValue("offset", m_offset); // offset: 112/132 size: 4/4 align: 4/4
    serializer.WriteValue("scale", m_scale);   // offset: 116/136 size: 4/4 align: 4/4
    serializer.Pad(16);                        // offset: 120/140 size: 8/4
    // class size: 128/144 align: 16/16
}

void nemesis::hkpCompressedSampledHeightFieldShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpSampledHeightFieldShape::DeserializeFrom(
        deserializer);                                      // offset: 0/0 size: 96/112 align: 16/16
    deserializer.ReadObject("storage", m_storage);          // offset: 96/112 size: 12/16 align: 4/8
    deserializer.ReadValue("triangleFlip", m_triangleFlip); // offset: 108/128 size: 1/1 align: 1/1
    deserializer.Skip(3);                                   // offset: 109/129 size: 3/3

    deserializer.ReadValue("offset", m_offset); // offset: 112/132 size: 4/4 align: 4/4
    deserializer.ReadValue("scale", m_scale);   // offset: 116/136 size: 4/4 align: 4/4
    deserializer.Pad(16);                       // offset: 120/140 size: 8/4
    // class size: 128/144 align: 16/16
}

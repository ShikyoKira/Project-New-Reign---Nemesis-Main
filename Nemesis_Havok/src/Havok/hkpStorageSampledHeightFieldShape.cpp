#include "Havok/hkpStorageSampledHeightFieldShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStorageSampledHeightFieldShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStorageSampledHeightFieldShape::Class; });

nemesis::hkpStorageSampledHeightFieldShape::hkpStorageSampledHeightFieldShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpSampledHeightFieldShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpStorageSampledHeightFieldShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStorageSampledHeightFieldShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpSampledHeightFieldShape::SerializeTo(serializer); // offset: 0/0 size: 96/112 align: 16/16
    serializer.WriteObject("storage", m_storage);                 // offset: 96/112 size: 12/16 align: 4/8
    serializer.WriteValue("triangleFlip", m_triangleFlip);        // offset: 108/128 size: 1/1 align: 1/1
    serializer.Pad(16);                                           // offset: 109/129 size: 3/15
    // class size: 112/144 align: 16/16
}

void nemesis::hkpStorageSampledHeightFieldShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpSampledHeightFieldShape::DeserializeFrom(
        deserializer);                                      // offset: 0/0 size: 96/112 align: 16/16
    deserializer.ReadObject("storage", m_storage);          // offset: 96/112 size: 12/16 align: 4/8
    deserializer.ReadValue("triangleFlip", m_triangleFlip); // offset: 108/128 size: 1/1 align: 1/1
    deserializer.Pad(16);                                   // offset: 109/129 size: 3/15
    // class size: 112/144 align: 16/16
}

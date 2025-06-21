#include "Havok/hkpCollidableBoundingVolumeData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCollidableBoundingVolumeData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCollidableBoundingVolumeData::Class; });

nemesis::hkpCollidableBoundingVolumeData::hkpCollidableBoundingVolumeData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpCollidableBoundingVolumeData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCollidableBoundingVolumeData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("min", m_min);                       // offset: 0/0 size: 12/12 align: 4/4
    serializer.WriteValue("expansionMin", m_expansionMin);     // offset: 12/12 size: 3/3 align: 1/1
    serializer.WriteValue("expansionShift", m_expansionShift); // offset: 15/15 size: 1/1 align: 1/1
    serializer.WriteValue("max", m_max);                       // offset: 16/16 size: 12/12 align: 4/4
    serializer.WriteValue("expansionMax", m_expansionMax);     // offset: 28/28 size: 3/3 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredValue("padding", m_padding); // offset: 31/31 size: 1/1 align: 1/1
    }
    else
    {
        serializer.WriteValue("padding", m_padding); // offset: 31/31 size: 1/1 align: 1/1
    }

    serializer.WriteSerializeIgnoredValue("numChildShapeAabbs",
                                          m_numChildShapeAabbs); // offset: 32/32 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("capacityChildShapeAabbs",
                                          m_capacityChildShapeAabbs); // offset: 34/34 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                      // offset: 36/36 size: 0/4

    serializer.WriteSerializeIgnoredObject("childShapeAabbs",
                                           m_childShapeAabbs); // offset: 36/40 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("childShapeKeys",
                                           m_childShapeKeys); // offset: 40/48 size: 4/8 align: 4/8
    // class size: 44/56 align: 4/8
}

void nemesis::hkpCollidableBoundingVolumeData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("min", m_min);                       // offset: 0/0 size: 12/12 align: 4/4
    deserializer.ReadValue("expansionMin", m_expansionMin);     // offset: 12/12 size: 3/3 align: 1/1
    deserializer.ReadValue("expansionShift", m_expansionShift); // offset: 15/15 size: 1/1 align: 1/1
    deserializer.ReadValue("max", m_max);                       // offset: 16/16 size: 12/12 align: 4/4
    deserializer.ReadValue("expansionMax", m_expansionMax);     // offset: 28/28 size: 3/3 align: 1/1
    deserializer.ReadValue("padding", m_padding);               // offset: 31/31 size: 1/1 align: 1/1
    deserializer.ReadValue("numChildShapeAabbs", m_numChildShapeAabbs); // offset: 32/32 size: 2/2 align: 2/2
    deserializer.ReadValue("capacityChildShapeAabbs",
                           m_capacityChildShapeAabbs); // offset: 34/34 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());   // offset: 36/36 size: 0/4

    deserializer.ReadObject("childShapeAabbs", m_childShapeAabbs); // offset: 36/40 size: 4/8 align: 4/8
    deserializer.ReadObject("childShapeKeys", m_childShapeKeys);   // offset: 40/48 size: 4/8 align: 4/8
    // class size: 44/56 align: 4/8
}

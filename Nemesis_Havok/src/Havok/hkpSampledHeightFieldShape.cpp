#include "Havok/hkpSampledHeightFieldShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSampledHeightFieldShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSampledHeightFieldShape::Class; });

nemesis::hkpSampledHeightFieldShape::hkpSampledHeightFieldShape() noexcept
    : nemesis::hkpSampledHeightFieldShape(Class)
{
}

nemesis::hkpSampledHeightFieldShape::hkpSampledHeightFieldShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpHeightFieldShape(cls)
{
}

const nemesis::hkClass* nemesis::hkpSampledHeightFieldShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSampledHeightFieldShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpHeightFieldShape::SerializeTo(serializer); // offset: 0/0 size: 16/32 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteObject("coarseTreeData", m_coarseTreeData);
        serializer.WriteValue("coarseness", m_coarseness);
        serializer.WriteValue("raycastMinY", m_raycastMinY);
        serializer.WriteValue("raycastMaxY", m_raycastMaxY);
    }

    serializer.WriteValue("xRes", m_xRes);                 // offset: 16/32 size: 4/4 align: 4/4
    serializer.WriteValue("zRes", m_zRes);                 // offset: 20/36 size: 4/4 align: 4/4
    serializer.WriteValue("heightCenter", m_heightCenter); // offset: 24/40 size: 4/4 align: 4/4
    serializer.WriteValue("useProjectionBasedHeight",
                          m_useProjectionBasedHeight);           // offset: 28/44 size: 1/1 align: 1/1
    serializer.WriteValue("heightfieldType", m_heightfieldType); // offset: 29/45 size: 1/1 align: 1/1
    serializer.Skip(2);                                          // offset: 30/46 size: 2/2

    serializer.WriteValue("intToFloatScale", m_intToFloatScale); // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("floatToIntScale", m_floatToIntScale); // offset: 48/64 size: 16/16 align: 16/16
    serializer.WriteValue("floatToIntOffsetFloorCorrected",
                          m_floatToIntOffsetFloorCorrected); // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteValue("extents", m_extents);             // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

void nemesis::hkpSampledHeightFieldShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpHeightFieldShape::DeserializeFrom(deserializer); // offset: 0/0 size: 16/32 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadObject("coarseTreeData", m_coarseTreeData);
        deserializer.ReadValue("coarseness", m_coarseness);
        deserializer.ReadValue("raycastMinY", m_raycastMinY);
        deserializer.ReadValue("raycastMaxY", m_raycastMaxY);
    }

    deserializer.ReadValue("xRes", m_xRes);                 // offset: 16/32 size: 4/4 align: 4/4
    deserializer.ReadValue("zRes", m_zRes);                 // offset: 20/36 size: 4/4 align: 4/4
    deserializer.ReadValue("heightCenter", m_heightCenter); // offset: 24/40 size: 4/4 align: 4/4
    deserializer.ReadValue("useProjectionBasedHeight",
                           m_useProjectionBasedHeight);           // offset: 28/44 size: 1/1 align: 1/1
    deserializer.ReadValue("heightfieldType", m_heightfieldType); // offset: 29/45 size: 1/1 align: 1/1
    deserializer.Skip(2);                                         // offset: 30/46 size: 2/2

    deserializer.ReadValue("intToFloatScale", m_intToFloatScale); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("floatToIntScale", m_floatToIntScale); // offset: 48/64 size: 16/16 align: 16/16
    deserializer.ReadValue("floatToIntOffsetFloorCorrected",
                           m_floatToIntOffsetFloorCorrected); // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadValue("extents", m_extents);             // offset: 80/96 size: 16/16 align: 16/16
    // class size: 96/112 align: 16/16
}

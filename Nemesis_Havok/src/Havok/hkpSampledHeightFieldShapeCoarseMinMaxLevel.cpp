#include "Havok/hkpSampledHeightFieldShapeCoarseMinMaxLevel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSampledHeightFieldShapeCoarseMinMaxLevel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::Class; });

nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::hkpSampledHeightFieldShapeCoarseMinMaxLevel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("minMaxData", m_minMaxData); // offset: 0/0 size: 12/16 align: 4/8
    serializer.WriteValue("xRes", m_xRes);              // offset: 12/16 size: 4/4 align: 4/4
    serializer.WriteValue("zRes", m_zRes);              // offset: 16/20 size: 4/4 align: 4/4
    // class size: 20/24 align: 4/8
}

void nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("minMaxData", m_minMaxData); // offset: 0/0 size: 12/16 align: 4/8
    deserializer.ReadValue("xRes", m_xRes);              // offset: 12/16 size: 4/4 align: 4/4
    deserializer.ReadValue("zRes", m_zRes);              // offset: 16/20 size: 4/4 align: 4/4
    // class size: 20/24 align: 4/8
}

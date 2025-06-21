#include "Havok/hkaSplineCompressedAnimationTrackCompressionParams.h"

nemesis::hkaSplineCompressedAnimationTrackCompressionParams::
    hkaSplineCompressedAnimationTrackCompressionParams() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSplineCompressedAnimationTrackCompressionParams::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSplineCompressedAnimationTrackCompressionParams::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteValue("rotationTolerance", m_rotationTolerance);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("translationTolerance", m_translationTolerance); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("scaleTolerance", m_scaleTolerance);             // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("floatingTolerance", m_floatingTolerance); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("rotationDegree", m_rotationDegree);       // offset: 16/16 size: 2/2 align: 2/2
    serializer.WriteValue("translationDegree", m_translationDegree); // offset: 18/18 size: 2/2 align: 2/2
    serializer.WriteValue("scaleDegree", m_scaleDegree);             // offset: 20/20 size: 2/2 align: 2/2
    serializer.WriteValue("floatingDegree", m_floatingDegree);       // offset: 22/22 size: 2/2 align: 2/2
    serializer.WriteValue("rotationQuantizationType",
                          m_rotationQuantizationType); // offset: 24/24 size: 1/1 align: 1/1
    serializer.WriteValue("translationQuantizationType",
                          m_translationQuantizationType); // offset: 25/25 size: 1/1 align: 1/1
    serializer.WriteValue("scaleQuantizationType",
                          m_scaleQuantizationType); // offset: 26/26 size: 1/1 align: 1/1
    serializer.WriteValue("floatQuantizationType",
                          m_floatQuantizationType); // offset: 27/27 size: 1/1 align: 1/1
    // class size: 28/28 align: 4/4
}

void nemesis::hkaSplineCompressedAnimationTrackCompressionParams::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("rotationTolerance", m_rotationTolerance); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("translationTolerance",
                           m_translationTolerance);                   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("scaleTolerance", m_scaleTolerance);       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("floatingTolerance", m_floatingTolerance); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("rotationDegree", m_rotationDegree);       // offset: 16/16 size: 2/2 align: 2/2
    deserializer.ReadValue("translationDegree", m_translationDegree); // offset: 18/18 size: 2/2 align: 2/2
    deserializer.ReadValue("scaleDegree", m_scaleDegree);             // offset: 20/20 size: 2/2 align: 2/2
    deserializer.ReadValue("floatingDegree", m_floatingDegree);       // offset: 22/22 size: 2/2 align: 2/2
    deserializer.ReadValue("rotationQuantizationType",
                           m_rotationQuantizationType); // offset: 24/24 size: 1/1 align: 1/1
    deserializer.ReadValue("translationQuantizationType",
                           m_translationQuantizationType); // offset: 25/25 size: 1/1 align: 1/1
    deserializer.ReadValue("scaleQuantizationType",
                           m_scaleQuantizationType); // offset: 26/26 size: 1/1 align: 1/1
    deserializer.ReadValue("floatQuantizationType",
                           m_floatQuantizationType); // offset: 27/27 size: 1/1 align: 1/1
    // class size: 28/28 align: 4/4
}

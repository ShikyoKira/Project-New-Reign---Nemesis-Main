#include "Havok/hkaSplineCompressedAnimationAnimationCompressionParams.h"

nemesis::hkaSplineCompressedAnimationAnimationCompressionParams::
    hkaSplineCompressedAnimationAnimationCompressionParams() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSplineCompressedAnimationAnimationCompressionParams::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSplineCompressedAnimationAnimationCompressionParams::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteValue("maxFramesPerBlock", m_maxFramesPerBlock); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("enableSampleSingleTracks",
                          m_enableSampleSingleTracks); // offset: 2/2 size: 1/1 align: 1/1
    serializer.Skip(1);                                // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

void nemesis::hkaSplineCompressedAnimationAnimationCompressionParams::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("maxFramesPerBlock", m_maxFramesPerBlock); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("enableSampleSingleTracks",
                           m_enableSampleSingleTracks); // offset: 2/2 size: 1/1 align: 1/1
    deserializer.Skip(1);                               // offset: 3/3 size: 1/1
    // class size: 4/4 align: 2/2
}

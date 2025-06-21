#include "Havok/hkaQuantizedAnimationTrackCompressionParams.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaQuantizedAnimationTrackCompressionParams,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaQuantizedAnimationTrackCompressionParams::Class; });

nemesis::hkaQuantizedAnimationTrackCompressionParams::hkaQuantizedAnimationTrackCompressionParams() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaQuantizedAnimationTrackCompressionParams::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaQuantizedAnimationTrackCompressionParams::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("rotationTolerance", m_rotationTolerance);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("translationTolerance", m_translationTolerance); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("scaleTolerance", m_scaleTolerance);             // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("floatingTolerance", m_floatingTolerance); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkaQuantizedAnimationTrackCompressionParams::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("rotationTolerance", m_rotationTolerance); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("translationTolerance",
                           m_translationTolerance);                   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("scaleTolerance", m_scaleTolerance);       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("floatingTolerance", m_floatingTolerance); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

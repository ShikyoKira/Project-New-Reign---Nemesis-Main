#include "Havok/hkaSplineCompressedAnimationPerTrackCompressionParams.h"

nemesis::hkaSplineCompressedAnimationPerTrackCompressionParams::
    hkaSplineCompressedAnimationPerTrackCompressionParams() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSplineCompressedAnimationPerTrackCompressionParams::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSplineCompressedAnimationPerTrackCompressionParams::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteObject("parameterPalette", m_parameterPalette); // offset: 0/0 size: 12/16 align: 4/8
    serializer.WriteObject("trackIndexToPaletteIndex",
                           m_trackIndexToPaletteIndex); // offset: 12/16 size: 12/16 align: 4/8
    serializer.WriteObject("floatTrackIndexToPaletteIndex",
                           m_floatTrackIndexToPaletteIndex); // offset: 24/32 size: 12/16 align: 4/8
    // class size: 36/48 align: 4/8
}

void nemesis::hkaSplineCompressedAnimationPerTrackCompressionParams::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("parameterPalette", m_parameterPalette); // offset: 0/0 size: 12/16 align: 4/8
    deserializer.ReadObject("trackIndexToPaletteIndex",
                            m_trackIndexToPaletteIndex); // offset: 12/16 size: 12/16 align: 4/8
    deserializer.ReadObject("floatTrackIndexToPaletteIndex",
                            m_floatTrackIndexToPaletteIndex); // offset: 24/32 size: 12/16 align: 4/8
    // class size: 36/48 align: 4/8
}

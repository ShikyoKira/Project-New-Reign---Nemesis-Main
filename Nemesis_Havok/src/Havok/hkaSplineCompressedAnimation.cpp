#include "Havok/hkaSplineCompressedAnimation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSplineCompressedAnimation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSplineCompressedAnimation::Class; });

nemesis::hkaSplineCompressedAnimation::hkaSplineCompressedAnimation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkaAnimation(Class)
{
}

const nemesis::hkClass* nemesis::hkaSplineCompressedAnimation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSplineCompressedAnimation::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkaAnimation::SerializeTo(serializer);                  // offset: 0/0 size: 40/56 align: 4/8
    serializer.WriteValue("numFrames", m_numFrames);                 // offset: 40/56 size: 4/4 align: 4/4
    serializer.WriteValue("numBlocks", m_numBlocks);                 // offset: 44/60 size: 4/4 align: 4/4
    serializer.WriteValue("maxFramesPerBlock", m_maxFramesPerBlock); // offset: 48/64 size: 4/4 align: 4/4
    serializer.WriteValue("maskAndQuantizationSize",
                          m_maskAndQuantizationSize);        // offset: 52/68 size: 4/4 align: 4/4
    serializer.WriteValue("blockDuration", m_blockDuration); // offset: 56/72 size: 4/4 align: 4/4
    serializer.WriteValue("blockInverseDuration",
                          m_blockInverseDuration);           // offset: 60/76 size: 4/4 align: 4/4
    serializer.WriteValue("frameDuration", m_frameDuration); // offset: 64/80 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                            // offset: 68/84 size: 0/4

    serializer.WriteObject("blockOffsets", m_blockOffsets);           // offset: 68/88 size: 12/16 align: 4/8
    serializer.WriteObject("floatBlockOffsets", m_floatBlockOffsets); // offset: 80/104 size: 12/16 align: 4/8
    serializer.WriteObject("transformOffsets", m_transformOffsets);   // offset: 92/120 size: 12/16 align: 4/8
    serializer.WriteObject("floatOffsets", m_floatOffsets); // offset: 104/136 size: 12/16 align: 4/8
    serializer.WriteObject("data", m_data);                 // offset: 116/152 size: 12/16 align: 4/8
    serializer.WriteValue("endian", m_endian);              // offset: 128/168 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                           // offset: 132/172 size: 0/4
    // class size: 132/176 align: 4/8
}

void nemesis::hkaSplineCompressedAnimation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkaAnimation::DeserializeFrom(deserializer);             // offset: 0/0 size: 40/56 align: 4/8
    deserializer.ReadValue("numFrames", m_numFrames);                 // offset: 40/56 size: 4/4 align: 4/4
    deserializer.ReadValue("numBlocks", m_numBlocks);                 // offset: 44/60 size: 4/4 align: 4/4
    deserializer.ReadValue("maxFramesPerBlock", m_maxFramesPerBlock); // offset: 48/64 size: 4/4 align: 4/4
    deserializer.ReadValue("maskAndQuantizationSize",
                           m_maskAndQuantizationSize);        // offset: 52/68 size: 4/4 align: 4/4
    deserializer.ReadValue("blockDuration", m_blockDuration); // offset: 56/72 size: 4/4 align: 4/4
    deserializer.ReadValue("blockInverseDuration",
                           m_blockInverseDuration);           // offset: 60/76 size: 4/4 align: 4/4
    deserializer.ReadValue("frameDuration", m_frameDuration); // offset: 64/80 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                           // offset: 68/84 size: 0/4

    deserializer.ReadObject("blockOffsets", m_blockOffsets); // offset: 68/88 size: 12/16 align: 4/8
    deserializer.ReadObject("floatBlockOffsets",
                            m_floatBlockOffsets);                    // offset: 80/104 size: 12/16 align: 4/8
    deserializer.ReadObject("transformOffsets", m_transformOffsets); // offset: 92/120 size: 12/16 align: 4/8
    deserializer.ReadObject("floatOffsets", m_floatOffsets);         // offset: 104/136 size: 12/16 align: 4/8
    deserializer.ReadObject("data", m_data);                         // offset: 116/152 size: 12/16 align: 4/8
    deserializer.ReadValue("endian", m_endian);                      // offset: 128/168 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                                  // offset: 132/172 size: 0/4
    // class size: 132/176 align: 4/8
}

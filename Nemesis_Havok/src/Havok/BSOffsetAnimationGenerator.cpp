#include "Havok/BSOffsetAnimationGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BSOffsetAnimationGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSOffsetAnimationGenerator::Class; });

nemesis::BSOffsetAnimationGenerator::BSOffsetAnimationGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BSOffsetAnimationGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSOffsetAnimationGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(16);                             // offset: 40/72 size: 8/8

    serializer.WriteObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    serializer.Pad(16);                                               // offset: 52/88 size: 12/8

    serializer.WriteObject("pOffsetClipGenerator",
                           m_pOffsetClipGenerator);                  // offset: 64/96 size: 4/8 align: 16/16
    serializer.WriteValue("fOffsetVariable", m_fOffsetVariable);     // offset: 68/104 size: 4/4 align: 4/4
    serializer.WriteValue("fOffsetRangeStart", m_fOffsetRangeStart); // offset: 72/108 size: 4/4 align: 4/4
    serializer.WriteValue("fOffsetRangeEnd", m_fOffsetRangeEnd);     // offset: 76/112 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                     // offset: 80/116 size: 0/4

    serializer.WriteSerializeIgnoredObject("BoneOffsetA",
                                           m_BoneOffsetA); // offset: 80/120 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("BoneIndexA",
                                           m_BoneIndexA); // offset: 92/136 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("fCurrentPercentage",
                                          m_fCurrentPercentage); // offset: 104/152 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("iCurrentFrame",
                                          m_iCurrentFrame); // offset: 108/156 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("bZeroOffset",
                                          m_bZeroOffset); // offset: 112/160 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bOffsetValid",
                                          m_bOffsetValid); // offset: 113/161 size: 1/1 align: 1/1
    serializer.Skip(14);                                   // offset: 114/162 size: 14/14
    // class size: 128/176 align: 16/16
}

void nemesis::BSOffsetAnimationGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(16);                                 // offset: 40/72 size: 8/8

    deserializer.ReadObject("pDefaultGenerator", m_pDefaultGenerator); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.Pad(16);                                              // offset: 52/88 size: 12/8

    deserializer.ReadObject("pOffsetClipGenerator",
                            m_pOffsetClipGenerator);                  // offset: 64/96 size: 4/8 align: 16/16
    deserializer.ReadValue("fOffsetVariable", m_fOffsetVariable);     // offset: 68/104 size: 4/4 align: 4/4
    deserializer.ReadValue("fOffsetRangeStart", m_fOffsetRangeStart); // offset: 72/108 size: 4/4 align: 4/4
    deserializer.ReadValue("fOffsetRangeEnd", m_fOffsetRangeEnd);     // offset: 76/112 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 80/116 size: 0/4

    deserializer.ReadObject("BoneOffsetA", m_BoneOffsetA); // offset: 80/120 size: 12/16 align: 4/8
    deserializer.ReadObject("BoneIndexA", m_BoneIndexA);   // offset: 92/136 size: 12/16 align: 4/8
    deserializer.ReadValue("fCurrentPercentage",
                           m_fCurrentPercentage);             // offset: 104/152 size: 4/4 align: 4/4
    deserializer.ReadValue("iCurrentFrame", m_iCurrentFrame); // offset: 108/156 size: 4/4 align: 4/4
    deserializer.ReadValue("bZeroOffset", m_bZeroOffset);     // offset: 112/160 size: 1/1 align: 1/1
    deserializer.ReadValue("bOffsetValid", m_bOffsetValid);   // offset: 113/161 size: 1/1 align: 1/1
    deserializer.Skip(14);                                    // offset: 114/162 size: 14/14
    // class size: 128/176 align: 16/16
}

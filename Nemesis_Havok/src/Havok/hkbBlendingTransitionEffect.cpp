#include "Havok/hkbBlendingTransitionEffect.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlendingTransitionEffect,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlendingTransitionEffect::Class; });

nemesis::hkbBlendingTransitionEffect::hkbBlendingTransitionEffect() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbTransitionEffect(Class)
{
}

const nemesis::hkClass* nemesis::hkbBlendingTransitionEffect::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlendingTransitionEffect::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbTransitionEffect::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("duration", m_duration);         // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("toGeneratorStartTimeFraction",
                          m_toGeneratorStartTimeFraction); // offset: 48/84 size: 4/4 align: 4/4

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("flags", m_flags); // offset: 52/88 size: 2/2 align: 2/2
    }

    serializer.WriteValue("endMode", m_endMode);           // offset: 54/90 size: 1/1 align: 1/1
    serializer.WriteValue("blendCurve", m_blendCurve);     // offset: 55/91 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                          // offset: 56/92 size: 0/4

    serializer.WriteSerializeIgnoredObject("fromGenerator",
                                           m_fromGenerator); // offset: 56/96 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("toGenerator",
                                           m_toGenerator); // offset: 60/104 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "characterPoseAtBeginningOfTransition",
        m_characterPoseAtBeginningOfTransition); // offset: 64/112 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("timeRemaining",
                                          m_timeRemaining); // offset: 76/128 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeInTransition",
                                          m_timeInTransition); // offset: 80/132 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteSerializeIgnoredValue("alignmentBone", m_alignmentBone);
    }

    serializer.WriteSerializeIgnoredValue("applySelfTransition",
                                          m_applySelfTransition); // offset: 84/136 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("initializeCharacterPose",
                                          m_initializeCharacterPose); // offset: 85/137 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                     // offset: 86/138 size: 2/6
    // class size: 88/144 align: 4/8
}

void nemesis::hkbBlendingTransitionEffect::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbTransitionEffect::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("duration", m_duration);              // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("toGeneratorStartTimeFraction",
                           m_toGeneratorStartTimeFraction); // offset: 48/84 size: 4/4 align: 4/4

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("flags", m_flags); // offset: 52/88 size: 2/2 align: 2/2
    }

    deserializer.ReadValue("endMode", m_endMode);           // offset: 54/90 size: 1/1 align: 1/1
    deserializer.ReadValue("blendCurve", m_blendCurve);     // offset: 55/91 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                         // offset: 56/92 size: 0/4

    deserializer.ReadObject("fromGenerator", m_fromGenerator); // offset: 56/96 size: 4/8 align: 4/8
    deserializer.ReadObject("toGenerator", m_toGenerator);     // offset: 60/104 size: 4/8 align: 4/8
    deserializer.ReadObject("characterPoseAtBeginningOfTransition",
                            m_characterPoseAtBeginningOfTransition); // offset: 64/112 size: 12/16 align: 4/8
    deserializer.ReadValue("timeRemaining", m_timeRemaining);        // offset: 76/128 size: 4/4 align: 4/4
    deserializer.ReadValue("timeInTransition", m_timeInTransition);  // offset: 80/132 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("alignmentBone", m_alignmentBone);
    }

    deserializer.ReadValue("applySelfTransition",
                           m_applySelfTransition); // offset: 84/136 size: 1/1 align: 1/1
    deserializer.ReadValue("initializeCharacterPose",
                           m_initializeCharacterPose); // offset: 85/137 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                    // offset: 86/138 size: 2/6
    // class size: 88/144 align: 4/8
}

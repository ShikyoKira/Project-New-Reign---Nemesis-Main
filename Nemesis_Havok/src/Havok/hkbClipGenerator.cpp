#include "Havok/hkbClipGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClipGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClipGenerator::Class; });

nemesis::hkbClipGenerator::hkbClipGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbClipGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClipGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(padding_size);

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("animationBundleName", m_animationBundleName);
    }

    serializer.WriteValue("animationName", m_animationName); // offset: 40/72 size: 4/8 align: 4/8
    serializer.WriteObject("triggers", m_triggers);          // offset: 44/80 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("userPartitionMask", m_userPartitionMask);
    }

    serializer.WriteValue("cropStartAmountLocalTime",
                          m_cropStartAmountLocalTime); // offset: 48/88 size: 4/4 align: 4/4
    serializer.WriteValue("cropEndAmountLocalTime",
                          m_cropEndAmountLocalTime);               // offset: 52/92 size: 4/4 align: 4/4
    serializer.WriteValue("startTime", m_startTime);               // offset: 56/96 size: 4/4 align: 4/4
    serializer.WriteValue("playbackSpeed", m_playbackSpeed);       // offset: 60/100 size: 4/4 align: 4/4
    serializer.WriteValue("enforcedDuration", m_enforcedDuration); // offset: 64/104 size: 4/4 align: 4/4
    serializer.WriteValue("userControlledTimeFraction",
                          m_userControlledTimeFraction); // offset: 68/108 size: 4/4 align: 4/4
    serializer.WriteValue("animationBindingIndex",
                          m_animationBindingIndex); // offset: 72/112 size: 2/2 align: 2/2
    serializer.WriteValue("mode", m_mode);          // offset: 74/114 size: 1/1 align: 1/1
    serializer.WriteValue("flags", m_flags);        // offset: 75/115 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                   // offset: 76/116 size: 0/4

    serializer.WriteSerializeIgnoredObject("animDatas", m_animDatas); // offset: 76/120 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("animationControl",
                                           m_animationControl); // offset: 88/136 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("originalTriggers",
                                           m_originalTriggers);         // offset: 92/144 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("mapperData", m_mapperData); // offset: 96/152 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("binding", m_binding); // offset: 100/160 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("mirroredAnimation",
                                           m_mirroredAnimation); // offset: 104/168 size: 4/8 align: 4/8
    serializer.Pad(16);                                          // offset: 108/176 size: 4/0

    serializer.WriteSerializeIgnoredValue("extractedMotion",
                                          m_extractedMotion); // offset: 112/176 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredObject("echos", m_echos); // offset: 160/224 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("localTime", m_localTime); // offset: 172/240 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("time", m_time);           // offset: 176/244 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "previousUserControlledTimeFraction",
        m_previousUserControlledTimeFraction);                         // offset: 180/248 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("bufferSize", m_bufferSize); // offset: 184/252 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("echoBufferSize",
                                          m_echoBufferSize); // offset: 188/256 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("atEnd", m_atEnd); // offset: 192/260 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("ignoreStartTime",
                                          m_ignoreStartTime); // offset: 193/261 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("pingPongBackward",
                                          m_pingPongBackward); // offset: 194/262 size: 1/1 align: 1/1
    serializer.Pad(16);                                        // offset: 195/263 size: 13/9
    // class size: 208/272 align: 16/16
}

void nemesis::hkbClipGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(padding_size);

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("animationBundleName", m_animationBundleName);
    }

    deserializer.ReadValue("animationName", m_animationName); // offset: 40/72 size: 4/8 align: 4/8
    deserializer.ReadObject("triggers", m_triggers);          // offset: 44/80 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("userPartitionMask", m_userPartitionMask);
    }

    deserializer.ReadValue("cropStartAmountLocalTime",
                           m_cropStartAmountLocalTime); // offset: 48/88 size: 4/4 align: 4/4
    deserializer.ReadValue("cropEndAmountLocalTime",
                           m_cropEndAmountLocalTime);               // offset: 52/92 size: 4/4 align: 4/4
    deserializer.ReadValue("startTime", m_startTime);               // offset: 56/96 size: 4/4 align: 4/4
    deserializer.ReadValue("playbackSpeed", m_playbackSpeed);       // offset: 60/100 size: 4/4 align: 4/4
    deserializer.ReadValue("enforcedDuration", m_enforcedDuration); // offset: 64/104 size: 4/4 align: 4/4
    deserializer.ReadValue("userControlledTimeFraction",
                           m_userControlledTimeFraction); // offset: 68/108 size: 4/4 align: 4/4
    deserializer.ReadValue("animationBindingIndex",
                           m_animationBindingIndex); // offset: 72/112 size: 2/2 align: 2/2
    deserializer.ReadValue("mode", m_mode);          // offset: 74/114 size: 1/1 align: 1/1
    deserializer.ReadValue("flags", m_flags);        // offset: 75/115 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                  // offset: 76/116 size: 0/4

    deserializer.ReadObject("animDatas", m_animDatas);               // offset: 76/120 size: 12/16 align: 4/8
    deserializer.ReadObject("animationControl", m_animationControl); // offset: 88/136 size: 4/8 align: 4/8
    deserializer.ReadObject("originalTriggers", m_originalTriggers); // offset: 92/144 size: 4/8 align: 4/8
    deserializer.ReadObject("mapperData", m_mapperData);             // offset: 96/152 size: 4/8 align: 4/8
    deserializer.ReadObject("binding", m_binding);                   // offset: 100/160 size: 4/8 align: 4/8
    deserializer.ReadObject("mirroredAnimation", m_mirroredAnimation); // offset: 104/168 size: 4/8 align: 4/8
    deserializer.Pad(16);                                              // offset: 108/176 size: 4/0

    deserializer.ReadValue("extractedMotion", m_extractedMotion); // offset: 112/176 size: 48/48 align: 16/16
    deserializer.ReadObject("echos", m_echos);                    // offset: 160/224 size: 12/16 align: 4/8
    deserializer.ReadValue("localTime", m_localTime);             // offset: 172/240 size: 4/4 align: 4/4
    deserializer.ReadValue("time", m_time);                       // offset: 176/244 size: 4/4 align: 4/4
    deserializer.ReadValue("previousUserControlledTimeFraction",
                           m_previousUserControlledTimeFraction);   // offset: 180/248 size: 4/4 align: 4/4
    deserializer.ReadValue("bufferSize", m_bufferSize);             // offset: 184/252 size: 4/4 align: 4/4
    deserializer.ReadValue("echoBufferSize", m_echoBufferSize);     // offset: 188/256 size: 4/4 align: 4/4
    deserializer.ReadValue("atEnd", m_atEnd);                       // offset: 192/260 size: 1/1 align: 1/1
    deserializer.ReadValue("ignoreStartTime", m_ignoreStartTime);   // offset: 193/261 size: 1/1 align: 1/1
    deserializer.ReadValue("pingPongBackward", m_pingPongBackward); // offset: 194/262 size: 1/1 align: 1/1
    deserializer.Pad(16);                                           // offset: 195/263 size: 13/9
    // class size: 208/272 align: 16/16
}

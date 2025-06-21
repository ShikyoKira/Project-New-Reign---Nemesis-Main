#include "Havok/hkbClipGeneratorInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClipGeneratorInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClipGeneratorInternalState::Class; });

nemesis::hkbClipGeneratorInternalState::hkbClipGeneratorInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbClipGeneratorInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClipGeneratorInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("extractedMotion", m_extractedMotion); // offset: 16/16 size: 48/48 align: 16/16
    serializer.WriteObject("echos", m_echos);                    // offset: 64/64 size: 12/16 align: 4/8
    serializer.WriteValue("localTime", m_localTime);             // offset: 76/80 size: 4/4 align: 4/4
    serializer.WriteValue("time", m_time);                       // offset: 80/84 size: 4/4 align: 4/4
    serializer.WriteValue("previousUserControlledTimeFraction",
                          m_previousUserControlledTimeFraction);   // offset: 84/88 size: 4/4 align: 4/4
    serializer.WriteValue("bufferSize", m_bufferSize);             // offset: 88/92 size: 4/4 align: 4/4
    serializer.WriteValue("echoBufferSize", m_echoBufferSize);     // offset: 92/96 size: 4/4 align: 4/4
    serializer.WriteValue("atEnd", m_atEnd);                       // offset: 96/100 size: 1/1 align: 1/1
    serializer.WriteValue("ignoreStartTime", m_ignoreStartTime);   // offset: 97/101 size: 1/1 align: 1/1
    serializer.WriteValue("pingPongBackward", m_pingPongBackward); // offset: 98/102 size: 1/1 align: 1/1
    serializer.Pad(16);                                            // offset: 99/103 size: 13/9
    // class size: 112/112 align: 16/16
}

void nemesis::hkbClipGeneratorInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("extractedMotion", m_extractedMotion); // offset: 16/16 size: 48/48 align: 16/16
    deserializer.ReadObject("echos", m_echos);                    // offset: 64/64 size: 12/16 align: 4/8
    deserializer.ReadValue("localTime", m_localTime);             // offset: 76/80 size: 4/4 align: 4/4
    deserializer.ReadValue("time", m_time);                       // offset: 80/84 size: 4/4 align: 4/4
    deserializer.ReadValue("previousUserControlledTimeFraction",
                           m_previousUserControlledTimeFraction);   // offset: 84/88 size: 4/4 align: 4/4
    deserializer.ReadValue("bufferSize", m_bufferSize);             // offset: 88/92 size: 4/4 align: 4/4
    deserializer.ReadValue("echoBufferSize", m_echoBufferSize);     // offset: 92/96 size: 4/4 align: 4/4
    deserializer.ReadValue("atEnd", m_atEnd);                       // offset: 96/100 size: 1/1 align: 1/1
    deserializer.ReadValue("ignoreStartTime", m_ignoreStartTime);   // offset: 97/101 size: 1/1 align: 1/1
    deserializer.ReadValue("pingPongBackward", m_pingPongBackward); // offset: 98/102 size: 1/1 align: 1/1
    deserializer.Pad(16);                                           // offset: 99/103 size: 13/9
    // class size: 112/112 align: 16/16
}

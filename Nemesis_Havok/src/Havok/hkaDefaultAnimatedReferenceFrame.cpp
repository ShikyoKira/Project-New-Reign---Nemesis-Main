#include "Havok/hkaDefaultAnimatedReferenceFrame.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaDefaultAnimatedReferenceFrame,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaDefaultAnimatedReferenceFrame::Class; });

nemesis::hkaDefaultAnimatedReferenceFrame::hkaDefaultAnimatedReferenceFrame() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkaAnimatedReferenceFrame(Class)
{
}

const nemesis::hkClass* nemesis::hkaDefaultAnimatedReferenceFrame::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaDefaultAnimatedReferenceFrame::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkaAnimatedReferenceFrame::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                          // offset: 8/16 size: 8/0

    serializer.WriteValue("up", m_up);             // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("forward", m_forward);   // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("duration", m_duration); // offset: 48/48 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());   // offset: 52/52 size: 0/4

    serializer.WriteObject("referenceFrameSamples",
                           m_referenceFrameSamples); // offset: 52/56 size: 12/16 align: 4/8
    serializer.Pad(16);                              // offset: 64/72 size: 0/8
    // class size: 64/80 align: 16/16
}

void nemesis::hkaDefaultAnimatedReferenceFrame::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkaAnimatedReferenceFrame::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                              // offset: 8/16 size: 8/0

    deserializer.ReadValue("up", m_up);              // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("forward", m_forward);    // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("duration", m_duration);  // offset: 48/48 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 52/52 size: 0/4

    deserializer.ReadObject("referenceFrameSamples",
                            m_referenceFrameSamples); // offset: 52/56 size: 12/16 align: 4/8
    deserializer.Pad(16);                             // offset: 64/72 size: 0/8
    // class size: 64/80 align: 16/16
}

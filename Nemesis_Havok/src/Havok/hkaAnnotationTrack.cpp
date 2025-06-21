#include "Havok/hkaAnnotationTrack.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaAnnotationTrack,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaAnnotationTrack::Class; });

nemesis::hkaAnnotationTrack::hkaAnnotationTrack() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaAnnotationTrack::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaAnnotationTrack::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("trackName", m_trackName);      // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("annotations", m_annotations); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}

void nemesis::hkaAnnotationTrack::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("trackName", m_trackName);      // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("annotations", m_annotations); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}

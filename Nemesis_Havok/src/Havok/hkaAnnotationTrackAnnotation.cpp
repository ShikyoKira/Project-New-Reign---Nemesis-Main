#include "Havok/hkaAnnotationTrackAnnotation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaAnnotationTrackAnnotation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaAnnotationTrackAnnotation::Class; });

nemesis::hkaAnnotationTrackAnnotation::hkaAnnotationTrackAnnotation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaAnnotationTrackAnnotation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaAnnotationTrackAnnotation::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("time", m_time);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 4/4 size: 0/4

    serializer.WriteValue("text", m_text); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkaAnnotationTrackAnnotation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("time", m_time);          // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 4/4 size: 0/4

    deserializer.ReadValue("text", m_text); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

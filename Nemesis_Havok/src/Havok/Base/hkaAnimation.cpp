#include "Havok/Base/hkaAnimation.h"

nemesis::hkaAnimation::hkaAnimation(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkaAnimation::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("type", m_type_1);
    }
    else
    {
        serializer.WriteValue("type", m_type_0); // offset: 8/16 size: 4/4 align: 4/4
    }

    serializer.WriteValue("duration", m_duration);        // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("numberOfTransformTracks",
                          m_numberOfTransformTracks);                    // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("numberOfFloatTracks", m_numberOfFloatTracks); // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteObject("extractedMotion", m_extractedMotion);        // offset: 24/32 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        serializer.WriteObject("annotationTracks",
                               m_annotationTracks_1); // offset: 28/40 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteObject("annotationTracks", m_annotationTracks_0);
        serializer.WriteValue("", m_numAnnotationTracks);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 40/56 align: 4/8
}

void nemesis::hkaAnimation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("type", m_type_1);
    }
    else
    {
        deserializer.ReadValue("type", m_type_0); // offset: 8/16 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("duration", m_duration);             // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("numberOfTransformTracks",
                           m_numberOfTransformTracks); // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("numberOfFloatTracks",
                           m_numberOfFloatTracks);                 // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadObject("extractedMotion", m_extractedMotion); // offset: 24/32 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        deserializer.ReadObject("annotationTracks",
                                m_annotationTracks_1); // offset: 28/40 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("annotationTracks", m_annotationTracks_0);
        deserializer.ReadValue("", m_numAnnotationTracks);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 40/56 align: 4/8
}

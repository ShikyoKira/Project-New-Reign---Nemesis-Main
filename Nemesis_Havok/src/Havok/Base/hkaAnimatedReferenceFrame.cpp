#include "Havok/Base/hkaAnimatedReferenceFrame.h"

nemesis::hkaAnimatedReferenceFrame::hkaAnimatedReferenceFrame(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkaAnimatedReferenceFrame::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    // (Speculated Version)
    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredValue("frameType", m_frameType);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

void nemesis::hkaAnimatedReferenceFrame::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    // (Speculated Version)
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("frameType", m_frameType);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

#include "Havok/hkxNodeAnnotationData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxNodeAnnotationData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxNodeAnnotationData::Class; });

nemesis::hkxNodeAnnotationData::hkxNodeAnnotationData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxNodeAnnotationData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxNodeAnnotationData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("time", m_time);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 4/4 size: 0/4

    serializer.WriteValue("description", m_description); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkxNodeAnnotationData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("time", m_time);          // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 4/4 size: 0/4

    deserializer.ReadValue("description", m_description); // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

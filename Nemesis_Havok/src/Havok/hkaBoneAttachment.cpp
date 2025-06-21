#include "Havok/hkaBoneAttachment.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaBoneAttachment,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaBoneAttachment::Class; });

nemesis::hkaBoneAttachment::hkaBoneAttachment() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaBoneAttachment::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaBoneAttachment::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("originalSkeletonName",
                          m_originalSkeletonName); // offset: 8/16 size: 4/8 align: 4/8
    serializer.Pad(16);                            // offset: 12/24 size: 4/8

    serializer.WriteValue("boneFromAttachment",
                          m_boneFromAttachment);        // offset: 16/32 size: 64/64 align: 16/16
    serializer.WriteObject("attachment", m_attachment); // offset: 80/96 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);              // offset: 84/104 size: 4/8 align: 4/8
    serializer.WriteValue("boneIndex", m_boneIndex);    // offset: 88/112 size: 2/2 align: 2/2
    serializer.Pad(16);                                 // offset: 90/114 size: 6/14
    // class size: 96/128 align: 16/16
}

void nemesis::hkaBoneAttachment::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("originalSkeletonName",
                           m_originalSkeletonName); // offset: 8/16 size: 4/8 align: 4/8
    deserializer.Pad(16);                           // offset: 12/24 size: 4/8

    deserializer.ReadValue("boneFromAttachment",
                           m_boneFromAttachment);        // offset: 16/32 size: 64/64 align: 16/16
    deserializer.ReadObject("attachment", m_attachment); // offset: 80/96 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);              // offset: 84/104 size: 4/8 align: 4/8
    deserializer.ReadValue("boneIndex", m_boneIndex);    // offset: 88/112 size: 2/2 align: 2/2
    deserializer.Pad(16);                                // offset: 90/114 size: 6/14
    // class size: 96/128 align: 16/16
}

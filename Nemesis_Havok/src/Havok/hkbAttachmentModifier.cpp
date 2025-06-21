#include "Havok/hkbAttachmentModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAttachmentModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAttachmentModifier::Class; });

nemesis::hkbAttachmentModifier::hkbAttachmentModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbAttachmentModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAttachmentModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("sendToAttacherOnAttach",
                           m_sendToAttacherOnAttach); // offset: 44/80 size: 8/16 align: 4/8
    serializer.WriteObject("sendToAttacheeOnAttach",
                           m_sendToAttacheeOnAttach); // offset: 52/96 size: 8/16 align: 4/8
    serializer.WriteObject("sendToAttacherOnDetach",
                           m_sendToAttacherOnDetach); // offset: 60/112 size: 8/16 align: 4/8
    serializer.WriteObject("sendToAttacheeOnDetach",
                           m_sendToAttacheeOnDetach);             // offset: 68/128 size: 8/16 align: 4/8
    serializer.WriteObject("attachmentSetup", m_attachmentSetup); // offset: 76/144 size: 4/8 align: 4/8
    serializer.WriteObject("attacherHandle", m_attacherHandle);   // offset: 80/152 size: 4/8 align: 4/8
    serializer.WriteObject("attacheeHandle", m_attacheeHandle);   // offset: 84/160 size: 4/8 align: 4/8
    serializer.WriteValue("attacheeLayer", m_attacheeLayer);      // offset: 88/168 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                  // offset: 92/172 size: 0/4

    serializer.WriteSerializeIgnoredObject("attacheeRB", m_attacheeRB); // offset: 92/176 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("oldMotionType",
                                          m_oldMotionType); // offset: 96/184 size: 1/1 align: 1/1
    serializer.Skip(3);                                     // offset: 97/185 size: 3/3

    serializer.WriteSerializeIgnoredValue("oldFilterInfo",
                                          m_oldFilterInfo); // offset: 100/188 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject("attachment",
                                           m_attachment); // offset: 104/192 size: 4/8 align: 4/8
    // class size: 108/200 align: 4/8
}

void nemesis::hkbAttachmentModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("sendToAttacherOnAttach",
                            m_sendToAttacherOnAttach); // offset: 44/80 size: 8/16 align: 4/8
    deserializer.ReadObject("sendToAttacheeOnAttach",
                            m_sendToAttacheeOnAttach); // offset: 52/96 size: 8/16 align: 4/8
    deserializer.ReadObject("sendToAttacherOnDetach",
                            m_sendToAttacherOnDetach); // offset: 60/112 size: 8/16 align: 4/8
    deserializer.ReadObject("sendToAttacheeOnDetach",
                            m_sendToAttacheeOnDetach);             // offset: 68/128 size: 8/16 align: 4/8
    deserializer.ReadObject("attachmentSetup", m_attachmentSetup); // offset: 76/144 size: 4/8 align: 4/8
    deserializer.ReadObject("attacherHandle", m_attacherHandle);   // offset: 80/152 size: 4/8 align: 4/8
    deserializer.ReadObject("attacheeHandle", m_attacheeHandle);   // offset: 84/160 size: 4/8 align: 4/8
    deserializer.ReadValue("attacheeLayer", m_attacheeLayer);      // offset: 88/168 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());               // offset: 92/172 size: 0/4

    deserializer.ReadObject("attacheeRB", m_attacheeRB);      // offset: 92/176 size: 4/8 align: 4/8
    deserializer.ReadValue("oldMotionType", m_oldMotionType); // offset: 96/184 size: 1/1 align: 1/1
    deserializer.Skip(3);                                     // offset: 97/185 size: 3/3

    deserializer.ReadValue("oldFilterInfo", m_oldFilterInfo); // offset: 100/188 size: 4/4 align: 4/4
    deserializer.ReadObject("attachment", m_attachment);      // offset: 104/192 size: 4/8 align: 4/8
    // class size: 108/200 align: 4/8
}

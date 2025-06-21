#include "Havok/hkbAttachmentSetup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAttachmentSetup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAttachmentSetup::Class; });

nemesis::hkbAttachmentSetup::hkbAttachmentSetup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbAttachmentSetup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAttachmentSetup::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("blendInTime", m_blendInTime);  // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("moveAttacherFraction",
                          m_moveAttacherFraction); // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("gain", m_gain);         // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("extrapolationTimeStep",
                          m_extrapolationTimeStep);                    // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("fixUpGain", m_fixUpGain);                   // offset: 24/32 size: 4/4 align: 4/4
    serializer.WriteValue("maxLinearDistance", m_maxLinearDistance);   // offset: 28/36 size: 4/4 align: 4/4
    serializer.WriteValue("maxAngularDistance", m_maxAngularDistance); // offset: 32/40 size: 4/4 align: 4/4
    serializer.WriteValue("attachmentType", m_attachmentType);         // offset: 36/44 size: 1/1 align: 1/1
    serializer.Skip(3);                                                // offset: 37/45 size: 3/3
    // class size: 40/48 align: 4/8
}

void nemesis::hkbAttachmentSetup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("blendInTime", m_blendInTime);       // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("moveAttacherFraction",
                           m_moveAttacherFraction); // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("gain", m_gain);         // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("extrapolationTimeStep",
                           m_extrapolationTimeStep);                    // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("fixUpGain", m_fixUpGain);                   // offset: 24/32 size: 4/4 align: 4/4
    deserializer.ReadValue("maxLinearDistance", m_maxLinearDistance);   // offset: 28/36 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAngularDistance", m_maxAngularDistance); // offset: 32/40 size: 4/4 align: 4/4
    deserializer.ReadValue("attachmentType", m_attachmentType);         // offset: 36/44 size: 1/1 align: 1/1
    deserializer.Skip(3);                                               // offset: 37/45 size: 3/3
    // class size: 40/48 align: 4/8
}

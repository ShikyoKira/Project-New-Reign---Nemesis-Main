#include "Havok/hkxCamera.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxCamera,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxCamera::Class; });

nemesis::hkxCamera::hkxCamera() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxCamera::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxCamera::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
        serializer.Pad(16);                                   // offset: 8/16 size: 8/0
    }

    serializer.WriteValue("from", m_from);             // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("focus", m_focus);           // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("up", m_up);                 // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("fov", m_fov);               // offset: 64/64 size: 4/4 align: 4/4
    serializer.WriteValue("far", m_far);               // offset: 68/68 size: 4/4 align: 4/4
    serializer.WriteValue("near", m_near);             // offset: 72/72 size: 4/4 align: 4/4
    serializer.WriteValue("leftHanded", m_leftHanded); // offset: 76/76 size: 1/1 align: 1/1
    serializer.Skip(3);                                // offset: 77/77 size: 3/3
    // class size: 80/80 align: 16/16
}

void nemesis::hkxCamera::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
        deserializer.Pad(16);                                       // offset: 8/16 size: 8/0
    }

    deserializer.ReadValue("from", m_from);             // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("focus", m_focus);           // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("up", m_up);                 // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("fov", m_fov);               // offset: 64/64 size: 4/4 align: 4/4
    deserializer.ReadValue("far", m_far);               // offset: 68/68 size: 4/4 align: 4/4
    deserializer.ReadValue("near", m_near);             // offset: 72/72 size: 4/4 align: 4/4
    deserializer.ReadValue("leftHanded", m_leftHanded); // offset: 76/76 size: 1/1 align: 1/1
    deserializer.Skip(3);                               // offset: 77/77 size: 3/3
    // class size: 80/80 align: 16/16
}

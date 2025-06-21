#include "Havok/hkbHandle.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandle,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandle::Class; });

nemesis::hkbHandle::hkbHandle() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbHandle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandle::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);              // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("frame", m_frame);                          // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("rigidBody", m_rigidBody);                  // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("character", m_character);                  // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteValue("animationBoneIndex", m_animationBoneIndex); // offset: 20/40 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                       // offset: 22/42 size: 2/6
    // class size: 24/48 align: 4/8
}

void nemesis::hkbHandle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);         // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("frame", m_frame);                          // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("rigidBody", m_rigidBody);                  // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("character", m_character);                  // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadValue("animationBoneIndex", m_animationBoneIndex); // offset: 20/40 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 22/42 size: 2/6
    // class size: 24/48 align: 4/8
}

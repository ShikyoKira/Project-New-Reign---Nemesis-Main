#include "Havok/hkbKeyframeBonesModifierKeyframeInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbKeyframeBonesModifierKeyframeInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbKeyframeBonesModifierKeyframeInfo::Class; });

nemesis::hkbKeyframeBonesModifierKeyframeInfo::hkbKeyframeBonesModifierKeyframeInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbKeyframeBonesModifierKeyframeInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbKeyframeBonesModifierKeyframeInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("keyframedPosition", m_keyframedPosition); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("keyframedRotation", m_keyframedRotation); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("boneIndex", m_boneIndex);                 // offset: 32/32 size: 2/2 align: 2/2
    serializer.WriteValue("isValid", m_isValid);                     // offset: 34/34 size: 1/1 align: 1/1
    serializer.Skip(13);                                             // offset: 35/35 size: 13/13
    // class size: 48/48 align: 16/16
}

void nemesis::hkbKeyframeBonesModifierKeyframeInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("keyframedPosition", m_keyframedPosition); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("keyframedRotation",
                           m_keyframedRotation);      // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("boneIndex", m_boneIndex); // offset: 32/32 size: 2/2 align: 2/2
    deserializer.ReadValue("isValid", m_isValid);     // offset: 34/34 size: 1/1 align: 1/1
    deserializer.Skip(13);                            // offset: 35/35 size: 13/13
    // class size: 48/48 align: 16/16
}

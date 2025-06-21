#include "Havok/hkbGetHandleOnBoneModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGetHandleOnBoneModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGetHandleOnBoneModifier::Class; });

nemesis::hkbGetHandleOnBoneModifier::hkbGetHandleOnBoneModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbGetHandleOnBoneModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGetHandleOnBoneModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                     // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("handleOut", m_handleOut);                  // offset: 44/80 size: 4/8 align: 4/8
    serializer.WriteValue("localFrameName", m_localFrameName);         // offset: 48/88 size: 4/8 align: 4/8
    serializer.WriteValue("ragdollBoneIndex", m_ragdollBoneIndex);     // offset: 52/96 size: 2/2 align: 2/2
    serializer.WriteValue("animationBoneIndex", m_animationBoneIndex); // offset: 54/98 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                       // offset: 56/100 size: 0/4
    // class size: 56/104 align: 4/8
}

void nemesis::hkbGetHandleOnBoneModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);                // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("handleOut", m_handleOut);                  // offset: 44/80 size: 4/8 align: 4/8
    deserializer.ReadValue("localFrameName", m_localFrameName);         // offset: 48/88 size: 4/8 align: 4/8
    deserializer.ReadValue("ragdollBoneIndex", m_ragdollBoneIndex);     // offset: 52/96 size: 2/2 align: 2/2
    deserializer.ReadValue("animationBoneIndex", m_animationBoneIndex); // offset: 54/98 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 56/100 size: 0/4
    // class size: 56/104 align: 4/8
}

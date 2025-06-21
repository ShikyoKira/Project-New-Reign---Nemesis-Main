#include "Havok/hkbRigidBodyRagdollControlsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRigidBodyRagdollControlsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRigidBodyRagdollControlsModifier::Class; });

nemesis::hkbRigidBodyRagdollControlsModifier::hkbRigidBodyRagdollControlsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbRigidBodyRagdollControlsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRigidBodyRagdollControlsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteObject("controlData", m_controlData); // offset: 48/80 size: 64/64 align: 16/16
    serializer.WriteObject("bones", m_bones);             // offset: 112/144 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("animationBlendFraction", m_animationBlendFraction);
    }

    serializer.Pad(16); // offset: 116/152 size: 12/8
    // class size: 128/160 align: 16/16
}

void nemesis::hkbRigidBodyRagdollControlsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadObject("controlData", m_controlData); // offset: 48/80 size: 64/64 align: 16/16
    deserializer.ReadObject("bones", m_bones);             // offset: 112/144 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("animationBlendFraction", m_animationBlendFraction);
    }

    deserializer.Pad(16); // offset: 116/152 size: 12/8
    // class size: 128/160 align: 16/16
}

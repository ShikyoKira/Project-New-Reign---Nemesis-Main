#include "Havok/hkbPoweredRagdollControlsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbPoweredRagdollControlsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbPoweredRagdollControlsModifier::Class; });

nemesis::hkbPoweredRagdollControlsModifier::hkbPoweredRagdollControlsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbPoweredRagdollControlsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbPoweredRagdollControlsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteObject("controlData", m_controlData); // offset: 48/80 size: 32/32 align: 16/16
    serializer.WriteObject("bones", m_bones);             // offset: 80/112 size: 4/8 align: 4/8
    serializer.WriteObject("worldFromModelModeData",
                           m_worldFromModelModeData);     // offset: 84/120 size: 8/8 align: 2/2
    serializer.WriteObject("boneWeights", m_boneWeights); // offset: 92/128 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("animationBlendFraction", m_animationBlendFraction);
    }

    serializer.Pad(16); // offset: 96/136 size: 0/8
    // class size: 96/144 align: 16/16
}

void nemesis::hkbPoweredRagdollControlsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadObject("controlData", m_controlData); // offset: 48/80 size: 32/32 align: 16/16
    deserializer.ReadObject("bones", m_bones);             // offset: 80/112 size: 4/8 align: 4/8
    deserializer.ReadObject("worldFromModelModeData",
                            m_worldFromModelModeData);     // offset: 84/120 size: 8/8 align: 2/2
    deserializer.ReadObject("boneWeights", m_boneWeights); // offset: 92/128 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("animationBlendFraction", m_animationBlendFraction);
    }

    deserializer.Pad(16); // offset: 96/136 size: 0/8
    // class size: 96/144 align: 16/16
}

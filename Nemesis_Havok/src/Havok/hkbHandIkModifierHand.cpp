#include "Havok/hkbHandIkModifierHand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkModifierHand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkModifierHand::Class; });

nemesis::hkbHandIkModifierHand::hkbHandIkModifierHand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbHandIkModifierHand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkModifierHand::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("elbowAxisLS", m_elbowAxisLS); // offset: 0/0 size: 16/16 align: 16/16
        serializer.WriteValue("backHandNormalLS",
                              m_backHandNormalLS); // offset: 16/16 size: 16/16 align: 16/16
    }

    serializer.WriteValue("handOffsetLS", m_handOffsetLS); // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue(cur_ver > HavokVersion::HK_2010_2_0 ? "handOrientationOffsetLS"
                                                              : "handOrienationOffsetLS",
                          m_handOrienationOffsetLS); // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("maxElbowAngleDegrees",
                          m_maxElbowAngleDegrees); // offset: 64/64 size: 4/4 align: 4/4
    serializer.WriteValue("minElbowAngleDegrees",
                          m_minElbowAngleDegrees);           // offset: 68/68 size: 4/4 align: 4/4
    serializer.WriteValue("shoulderIndex", m_shoulderIndex); // offset: 72/72 size: 2/2 align: 2/2
    serializer.WriteValue("shoulderSiblingIndex",
                          m_shoulderSiblingIndex);                     // offset: 74/74 size: 2/2 align: 2/2
    serializer.WriteValue("elbowIndex", m_elbowIndex);                 // offset: 76/76 size: 2/2 align: 2/2
    serializer.WriteValue("elbowSiblingIndex", m_elbowSiblingIndex);   // offset: 78/78 size: 2/2 align: 2/2
    serializer.WriteValue("wristIndex", m_wristIndex);                 // offset: 80/80 size: 2/2 align: 2/2
    serializer.WriteValue("enforceEndPosition", m_enforceEndPosition); // offset: 82/82 size: 1/1 align: 1/1
    serializer.WriteValue("enforceEndRotation", m_enforceEndRotation); // offset: 83/83 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 84/84 size: 0/4

    serializer.WriteValue("localFrameName", m_localFrameName); // offset: 84/88 size: 4/8 align: 4/8
    serializer.Pad(16);                                        // offset: 88/96 size: 8/0
    // class size: 96/96 align: 16/16
}

void nemesis::hkbHandIkModifierHand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("elbowAxisLS", m_elbowAxisLS); // offset: 0/0 size: 16/16 align: 16/16
        deserializer.ReadValue("backHandNormalLS",
                               m_backHandNormalLS); // offset: 16/16 size: 16/16 align: 16/16
    }

    deserializer.ReadValue("handOffsetLS", m_handOffsetLS); // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue(cur_ver > HavokVersion::HK_2010_2_0 ? "handOrientationOffsetLS"
                                                               : "handOrienationOffsetLS",
                           m_handOrienationOffsetLS); // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("maxElbowAngleDegrees",
                           m_maxElbowAngleDegrees); // offset: 64/64 size: 4/4 align: 4/4
    deserializer.ReadValue("minElbowAngleDegrees",
                           m_minElbowAngleDegrees);           // offset: 68/68 size: 4/4 align: 4/4
    deserializer.ReadValue("shoulderIndex", m_shoulderIndex); // offset: 72/72 size: 2/2 align: 2/2
    deserializer.ReadValue("shoulderSiblingIndex",
                           m_shoulderSiblingIndex);                     // offset: 74/74 size: 2/2 align: 2/2
    deserializer.ReadValue("elbowIndex", m_elbowIndex);                 // offset: 76/76 size: 2/2 align: 2/2
    deserializer.ReadValue("elbowSiblingIndex", m_elbowSiblingIndex);   // offset: 78/78 size: 2/2 align: 2/2
    deserializer.ReadValue("wristIndex", m_wristIndex);                 // offset: 80/80 size: 2/2 align: 2/2
    deserializer.ReadValue("enforceEndPosition", m_enforceEndPosition); // offset: 82/82 size: 1/1 align: 1/1
    deserializer.ReadValue("enforceEndRotation", m_enforceEndRotation); // offset: 83/83 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 84/84 size: 0/4

    deserializer.ReadValue("localFrameName", m_localFrameName); // offset: 84/88 size: 4/8 align: 4/8
    deserializer.Pad(16);                                       // offset: 88/96 size: 8/0
    // class size: 96/96 align: 16/16
}

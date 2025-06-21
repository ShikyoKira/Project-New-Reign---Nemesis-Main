#include "Havok/hkbLookAtModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbLookAtModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbLookAtModifier::Class; });

nemesis::hkbLookAtModifier::hkbLookAtModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbLookAtModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbLookAtModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("targetWS", m_targetWS); // offset: 48/80 size: 16/16 align: 16/16

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("headForwardLS", m_headForwardLS); // offset: 64/96 size: 16/16 align: 16/16
        serializer.WriteValue("neckForwardLS", m_neckForwardLS); // offset: 80/112 size: 16/16 align: 16/16
        serializer.WriteValue("neckRightLS", m_neckRightLS);     // offset: 96/128 size: 16/16 align: 16/16
        serializer.WriteValue("eyePositionHS", m_eyePositionHS); // offset: 112/144 size: 16/16 align: 16/16
    }
    else
    {
        serializer.WriteValue("headForwardHS", m_headForwardLS);
        serializer.WriteValue("headRightHS", m_neckRightLS);
    }

    serializer.WriteValue("newTargetGain", m_newTargetGain);           // offset: 128/160 size: 4/4 align: 4/4
    serializer.WriteValue("onGain", m_onGain);                         // offset: 132/164 size: 4/4 align: 4/4
    serializer.WriteValue("offGain", m_offGain);                       // offset: 136/168 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleDegrees", m_limitAngleDegrees);   // offset: 140/172 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleLeft", m_limitAngleLeft);         // offset: 144/176 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleRight", m_limitAngleRight);       // offset: 148/180 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleUp", m_limitAngleUp);             // offset: 152/184 size: 4/4 align: 4/4
    serializer.WriteValue("limitAngleDown", m_limitAngleDown);         // offset: 156/188 size: 4/4 align: 4/4
    serializer.WriteValue("headIndex", m_headIndex);                   // offset: 160/192 size: 2/2 align: 2/2
    serializer.WriteValue("neckIndex", m_neckIndex);                   // offset: 162/194 size: 2/2 align: 2/2
    serializer.WriteValue("isOn", m_isOn);                             // offset: 164/196 size: 1/1 align: 1/1
    serializer.WriteValue("individualLimitsOn", m_individualLimitsOn); // offset: 165/197 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("isTargetInsideLimitCone",
                              m_isTargetInsideLimitCone); // offset: 166/198 size: 1/1 align: 1/1
    }

    serializer.Pad(16); // offset: 167/199 size: 9/9

    serializer.WriteSerializeIgnoredValue("lookAtLastTargetWS",
                                          m_lookAtLastTargetWS); // offset: 176/208 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("lookAtWeight",
                                          m_lookAtWeight); // offset: 192/224 size: 4/4 align: 4/4
    serializer.Pad(16);                                    // offset: 196/228 size: 12/12
    // class size: 208/240 align: 16/16
}

void nemesis::hkbLookAtModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("targetWS", m_targetWS); // offset: 48/80 size: 16/16 align: 16/16

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("headForwardLS", m_headForwardLS); // offset: 64/96 size: 16/16 align: 16/16
        deserializer.ReadValue("neckForwardLS", m_neckForwardLS); // offset: 80/112 size: 16/16 align: 16/16
        deserializer.ReadValue("neckRightLS", m_neckRightLS);     // offset: 96/128 size: 16/16 align: 16/16
        deserializer.ReadValue("eyePositionHS", m_eyePositionHS); // offset: 112/144 size: 16/16 align: 16/16
    }
    else
    {
        deserializer.ReadValue("headForwardHS", m_headForwardLS);
        deserializer.ReadValue("headRightHS", m_neckRightLS);
    }

    deserializer.ReadValue("newTargetGain", m_newTargetGain);         // offset: 128/160 size: 4/4 align: 4/4
    deserializer.ReadValue("onGain", m_onGain);                       // offset: 132/164 size: 4/4 align: 4/4
    deserializer.ReadValue("offGain", m_offGain);                     // offset: 136/168 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 140/172 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleLeft", m_limitAngleLeft);       // offset: 144/176 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleRight", m_limitAngleRight);     // offset: 148/180 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleUp", m_limitAngleUp);           // offset: 152/184 size: 4/4 align: 4/4
    deserializer.ReadValue("limitAngleDown", m_limitAngleDown);       // offset: 156/188 size: 4/4 align: 4/4
    deserializer.ReadValue("headIndex", m_headIndex);                 // offset: 160/192 size: 2/2 align: 2/2
    deserializer.ReadValue("neckIndex", m_neckIndex);                 // offset: 162/194 size: 2/2 align: 2/2
    deserializer.ReadValue("isOn", m_isOn);                           // offset: 164/196 size: 1/1 align: 1/1
    deserializer.ReadValue("individualLimitsOn",
                           m_individualLimitsOn); // offset: 165/197 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("isTargetInsideLimitCone",
                               m_isTargetInsideLimitCone); // offset: 166/198 size: 1/1 align: 1/1
    }

    deserializer.Pad(16); // offset: 167/199 size: 9/9

    deserializer.ReadValue("lookAtLastTargetWS",
                           m_lookAtLastTargetWS);           // offset: 176/208 size: 16/16 align: 16/16
    deserializer.ReadValue("lookAtWeight", m_lookAtWeight); // offset: 192/224 size: 4/4 align: 4/4
    deserializer.Pad(16);                                   // offset: 196/228 size: 12/12
    // class size: 208/240 align: 16/16
}

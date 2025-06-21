#include "Havok/hkbSenseHandleModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSenseHandleModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSenseHandleModifier::Class; });

nemesis::hkbSenseHandleModifier::hkbSenseHandleModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbSenseHandleModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSenseHandleModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);              // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteSerializeIgnoredObject("handle", m_handle); // offset: 44/80 size: 24/48 align: 4/8
    serializer.Pad(16);                                         // offset: 68/128 size: 12/0

    serializer.WriteValue("sensorLocalOffset",
                          m_sensorLocalOffset);                // offset: 80/128 size: 16/16 align: 16/16
    serializer.WriteObject("ranges", m_ranges);                // offset: 96/144 size: 12/16 align: 4/8
    serializer.WriteObject("handleOut", m_handleOut);          // offset: 108/160 size: 4/8 align: 4/8
    serializer.WriteObject("handleIn", m_handleIn);            // offset: 112/168 size: 4/8 align: 4/8
    serializer.WriteValue("localFrameName", m_localFrameName); // offset: 116/176 size: 4/8 align: 4/8
    serializer.WriteValue("sensorLocalFrameName",
                          m_sensorLocalFrameName);       // offset: 120/184 size: 4/8 align: 4/8
    serializer.WriteValue("minDistance", m_minDistance); // offset: 124/192 size: 4/4 align: 4/4
    serializer.WriteValue("maxDistance", m_maxDistance); // offset: 128/196 size: 4/4 align: 4/4
    serializer.WriteValue("distanceOut", m_distanceOut); // offset: 132/200 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo",
                          m_collisionFilterInfo); // offset: 136/204 size: 4/4 align: 4/4
    serializer.WriteValue("sensorRagdollBoneIndex",
                          m_sensorRagdollBoneIndex); // offset: 140/208 size: 2/2 align: 2/2
    serializer.WriteValue("sensorAnimationBoneIndex",
                          m_sensorAnimationBoneIndex);   // offset: 142/210 size: 2/2 align: 2/2
    serializer.WriteValue("sensingMode", m_sensingMode); // offset: 144/212 size: 1/1 align: 1/1
    serializer.WriteValue("extrapolateSensorPosition",
                          m_extrapolateSensorPosition); // offset: 145/213 size: 1/1 align: 1/1
    serializer.WriteValue("keepFirstSensedHandle",
                          m_keepFirstSensedHandle);            // offset: 146/214 size: 1/1 align: 1/1
    serializer.WriteValue("foundHandleOut", m_foundHandleOut); // offset: 147/215 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("timeSinceLastModify",
                                          m_timeSinceLastModify); // offset: 148/216 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "rangeIndexForEventToSendNextUpdate",
        m_rangeIndexForEventToSendNextUpdate); // offset: 152/220 size: 4/4 align: 4/4
    serializer.Pad(16);                        // offset: 156/224 size: 4/0
    // class size: 160/224 align: 16/16
}

void nemesis::hkbSenseHandleModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("handle", m_handle);         // offset: 44/80 size: 24/48 align: 4/8
    deserializer.Pad(16);                                // offset: 68/128 size: 12/0

    deserializer.ReadValue("sensorLocalOffset",
                           m_sensorLocalOffset);                // offset: 80/128 size: 16/16 align: 16/16
    deserializer.ReadObject("ranges", m_ranges);                // offset: 96/144 size: 12/16 align: 4/8
    deserializer.ReadObject("handleOut", m_handleOut);          // offset: 108/160 size: 4/8 align: 4/8
    deserializer.ReadObject("handleIn", m_handleIn);            // offset: 112/168 size: 4/8 align: 4/8
    deserializer.ReadValue("localFrameName", m_localFrameName); // offset: 116/176 size: 4/8 align: 4/8
    deserializer.ReadValue("sensorLocalFrameName",
                           m_sensorLocalFrameName);       // offset: 120/184 size: 4/8 align: 4/8
    deserializer.ReadValue("minDistance", m_minDistance); // offset: 124/192 size: 4/4 align: 4/4
    deserializer.ReadValue("maxDistance", m_maxDistance); // offset: 128/196 size: 4/4 align: 4/4
    deserializer.ReadValue("distanceOut", m_distanceOut); // offset: 132/200 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo",
                           m_collisionFilterInfo); // offset: 136/204 size: 4/4 align: 4/4
    deserializer.ReadValue("sensorRagdollBoneIndex",
                           m_sensorRagdollBoneIndex); // offset: 140/208 size: 2/2 align: 2/2
    deserializer.ReadValue("sensorAnimationBoneIndex",
                           m_sensorAnimationBoneIndex);   // offset: 142/210 size: 2/2 align: 2/2
    deserializer.ReadValue("sensingMode", m_sensingMode); // offset: 144/212 size: 1/1 align: 1/1
    deserializer.ReadValue("extrapolateSensorPosition",
                           m_extrapolateSensorPosition); // offset: 145/213 size: 1/1 align: 1/1
    deserializer.ReadValue("keepFirstSensedHandle",
                           m_keepFirstSensedHandle);            // offset: 146/214 size: 1/1 align: 1/1
    deserializer.ReadValue("foundHandleOut", m_foundHandleOut); // offset: 147/215 size: 1/1 align: 1/1
    deserializer.ReadValue("timeSinceLastModify",
                           m_timeSinceLastModify); // offset: 148/216 size: 4/4 align: 4/4
    deserializer.ReadValue("rangeIndexForEventToSendNextUpdate",
                           m_rangeIndexForEventToSendNextUpdate); // offset: 152/220 size: 4/4 align: 4/4
    deserializer.Pad(16);                                         // offset: 156/224 size: 4/0
    // class size: 160/224 align: 16/16
}

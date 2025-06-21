#include "Havok/hkbGetUpModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGetUpModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGetUpModifier::Class; });

nemesis::hkbGetUpModifier::hkbGetUpModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbGetUpModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGetUpModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("groundNormal", m_groundNormal); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("duration", m_duration);         // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("alignWithGroundDuration",
                          m_alignWithGroundDuration);              // offset: 68/100 size: 4/4 align: 4/4
    serializer.WriteValue("rootBoneIndex", m_rootBoneIndex);       // offset: 72/104 size: 2/2 align: 2/2
    serializer.WriteValue("otherBoneIndex", m_otherBoneIndex);     // offset: 74/106 size: 2/2 align: 2/2
    serializer.WriteValue("anotherBoneIndex", m_anotherBoneIndex); // offset: 76/108 size: 2/2 align: 2/2
    serializer.Skip(2);                                            // offset: 78/110 size: 2/2

    serializer.WriteSerializeIgnoredValue("timeSinceBegin",
                                          m_timeSinceBegin);       // offset: 80/112 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep); // offset: 84/116 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("initNextModify",
                                          m_initNextModify); // offset: 88/120 size: 1/1 align: 1/1
    serializer.Skip(7);                                      // offset: 89/121 size: 7/7
    // class size: 96/128 align: 16/16
}

void nemesis::hkbGetUpModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("groundNormal", m_groundNormal); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("duration", m_duration);         // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("alignWithGroundDuration",
                           m_alignWithGroundDuration);              // offset: 68/100 size: 4/4 align: 4/4
    deserializer.ReadValue("rootBoneIndex", m_rootBoneIndex);       // offset: 72/104 size: 2/2 align: 2/2
    deserializer.ReadValue("otherBoneIndex", m_otherBoneIndex);     // offset: 74/106 size: 2/2 align: 2/2
    deserializer.ReadValue("anotherBoneIndex", m_anotherBoneIndex); // offset: 76/108 size: 2/2 align: 2/2
    deserializer.Skip(2);                                           // offset: 78/110 size: 2/2

    deserializer.ReadValue("timeSinceBegin", m_timeSinceBegin); // offset: 80/112 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);             // offset: 84/116 size: 4/4 align: 4/4
    deserializer.ReadValue("initNextModify", m_initNextModify); // offset: 88/120 size: 1/1 align: 1/1
    deserializer.Skip(7);                                       // offset: 89/121 size: 7/7
    // class size: 96/128 align: 16/16
}
